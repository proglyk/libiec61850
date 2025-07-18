#include "libiec61850/IsoSession.h"
#include "libiec61850/IsoPresentation.h"
#include <string.h>

// Type definitions

struct sIsoSession {
  // Own needs
  u16_t      callingSessionSelector;
	u16_t      calledSessionSelector;
	u16_t      sessionRequirement;
	u8_t       protocolOptions;
	ByteBuffer userData;
  // Linkage with the upper layer
  IsoPresPtr isoPres;
  SBufferPtr sbuf;
};

static IsoSessStatus IsoSession_parseMessage(IsoSessPtr, ByteBuffer *);
static ByteBuffer *IsoSession_getUserData(IsoSessPtr);
static long	createSDataSpdu(SBuffer *);
static IsoSessStatus parseSessionHeaderParameters(IsoSessPtr, ByteBuffer *, s32_t);
static s32_t parseAcceptParameters(IsoSessPtr, ByteBuffer *, s32_t, s32_t);
static void	createSAcceptSpdu(IsoSessPtr, SBuffer *);
// 4
static s32_t encodeConnectAcceptItem(u8_t *, s32_t, u8_t);
static s32_t encodeSessionRequirement(IsoSessPtr, u8_t *, s32_t);
static s32_t encodeCalledSessionSelector(IsoSessPtr, u8_t *, s32_t);
static s32_t encodeSessionUserData(IsoSessPtr, u8_t *, s32_t, u8_t);

/**	----------------------------------------------------------------------------
	* @brief Iso Session layer constructor */
IsoSessPtr
	IsoSession_Create(SBufferPtr sbuf) {
/*----------------------------------------------------------------------------*/
	// Self creating
  IsoSessPtr self = calloc(1, sizeof(struct sIsoSession));
  if (!self) return NULL;
	self->sessionRequirement = 0x0002; /* default = duplex functional unit */
	self->callingSessionSelector = 0x0001;
	self->calledSessionSelector = 0x0001;
  // линкуем SBuffer
  self->sbuf = sbuf;
  // Top layers creating
  self->isoPres = IsoPresentation_Create(self->sbuf);
  if (!self->isoPres) return NULL;
  
  return self;
}

/**	----------------------------------------------------------------------------
	* @brief Iso Session layer destructor */
void
  IsoSession_Delete(IsoSessPtr self) {
/*----------------------------------------------------------------------------*/
	if (!self) return;
  if (self->isoPres) IsoPresentation_Delete(self->isoPres);
	free(self); self = NULL;
}

/**	----------------------------------------------------------------------------
	* @brief Iso Session layer action */
s32_t
  IsoSession_Process(IsoSessPtr self, ByteBuffer *buf) {
/*----------------------------------------------------------------------------*/
  IsoSessStatus sta;
  ByteBuffer* sessionUserData;
  s32_t rc;
  // read body
  sta = IsoSession_parseMessage(self, buf);
  sessionUserData = IsoSession_getUserData(self);
  // см.протокол.
  if (sta == SESSION_CONNECT) {
    rc = IsoPresentation_Connect(self->isoPres, sessionUserData);
    if (rc < 0) goto exit;
    createSAcceptSpdu(self, self->sbuf);
  }
  // см.протокол.
  else if (sta == SESSION_DATA) {
    rc = IsoPresentation_ProcessData(self->isoPres, sessionUserData);
    if (rc < 0) goto exit;
    createSDataSpdu(self->sbuf);
  }
  // Exception
  else {
    goto exit;
  }
  
  
  return 0;
  
  exit:
  //self->state = SESSION_ERROR;
  return -1;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
void
	IsoSession_ThrowOverListener( IsoSessPtr self, PassedHandlerPtr handler, void *param ) {
/*----------------------------------------------------------------------------*/
  IsoPresentation_InstallListener(self->isoPres, handler, param);
  IsoPresentation_ThrowOverListener(self->isoPres, handler, param);
}

// Определения локальных (private) функций

/**	----------------------------------------------------------------------------
	* @brief Iso Session layer action */
static IsoSessStatus
  IsoSession_parseMessage(IsoSessPtr self, ByteBuffer* message) {
/*----------------------------------------------------------------------------*/
	u8_t* buffer = message->buffer;
	u8_t id;
	u8_t length;

	//session->userData = NULL;

	 if (message->size > 1) {
		id = buffer[0];
		length = buffer[1];
	}
	else
		return SESSION_ERROR;

	switch (id) {
	case 13: // CONNECT(CN) SPDU

//		LWIP_DEBUGF(COTP_DEBUG, ("CONNECT(CN) SPDU\r\n"));
		if (length != (message->size - 2)){
//			LWIP_DEBUGF(COTP_DEBUG, ("SPDU Lenght - Bad\r\n"));
			return SESSION_ERROR;
		}
		if (parseSessionHeaderParameters(self, message, length) == SESSION_OK){
//			LWIP_DEBUGF(COTP_DEBUG, ("SPDU CONNECT - OK\r\n"));
			return SESSION_CONNECT;
		}
		else {
			//printf("iso_session.c: error parsing connect spdu\r\n");
			return SESSION_ERROR;
		}

		break;
	case 14: // ACCEPT SPDU
/*		if (length != (message->size - 2))
			return SESSION_ERROR;
		if (parseSessionHeaderParameters(session, message, length) == SESSION_OK)
			return SESSION_CONNECT;
		else {
			printf("iso_session.c: error parsing accept spdu\r\n");
			return SESSION_ERROR;
		}*/

 		break;
	case 1: // Give token / data SPDU
		if (message->size < 4)
			return SESSION_ERROR;
		if ((length == 0) && (buffer[2] == 1) && (buffer[3] == 0)) {
			//ByteBuffer* userData = calloc(1, sizeof(ByteBuffer));
			ByteBuffer_wrap(&self->userData, message->buffer + 4, message->size - 4, message->maxSize - 4);

			//session->userData = userData;
			return SESSION_DATA;
		}
		return SESSION_ERROR;

		break;
	}

	return SESSION_ERROR;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static ByteBuffer*
  IsoSession_getUserData(IsoSessPtr self) {
/*----------------------------------------------------------------------------*/
	return &self->userData;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static long
	createSDataSpdu(SBuffer* buffer) {
/*----------------------------------------------------------------------------*/
	SBuffer_SetToFront(buffer, 0x01);/* GiveToken SPDU */
	SBuffer_SetToFront(buffer, 0x00);
	SBuffer_SetToFront(buffer, 0x01); /* Data SPDU */
	SBuffer_SetToFront(buffer, 0x00);
	
	SBuffer_Shift(buffer);
	return 1;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static IsoSessStatus
  parseSessionHeaderParameters( IsoSessPtr self, ByteBuffer* message,
                                s32_t parametersOctets) {
/*----------------------------------------------------------------------------*/
	s32_t offset = 2;
	u8_t pgi;
	u8_t parameterLength;
	s32_t hasCallingSessionSelector = 0;
	s32_t hasCalledSessionSelector = 0;
	s32_t hasConnectAcceptItem = 0;
	s32_t hasSessionRequirement = 0;

	while (offset < (parametersOctets + 2)) {
		pgi = message->buffer[offset++];
		parameterLength = message->buffer[offset++];

		switch (pgi) {
		case 1:		// Connection Identifier
			//if (DEBUG) printf("iso_session: PGI - connection identifier\r\n");
			//printf("TODO: PGI not implemented!");
//			LWIP_DEBUGF(COTP_DEBUG, ("SESSION: pgi==1\r\n"));
			offset += parameterLength;
			break;
		case 5:		/* Connection/Accept Item */
//			if (COTP_DEBUG) printf("iso_session: PGI - Connection/Accept Item\r\n");
			s32_t connectAcceptLen;

			connectAcceptLen = parseAcceptParameters(self, message, offset,
				parameterLength);

			if (connectAcceptLen == -1)
				return SESSION_ERROR;

			offset += connectAcceptLen;
			hasConnectAcceptItem = 1;
			break;
		case 20: /* Session User Requirements */
//			if (COTP_DEBUG) printf("iso_session: PI - Session User Req\r\n");

			if (parameterLength != 2)
				return SESSION_ERROR;
			hasSessionRequirement = 1;
			self->sessionRequirement = message->buffer[offset++] * 0x100;
			self->sessionRequirement += message->buffer[offset++];
			break;
		case 51: /* Calling Session Selector */
/*			if (DEBUG) printf("iso_session: Parameter - Calling Session Selector\r\n");
			if (parameterLength != 2)
				return SESSION_ERROR;
			hasCallingSessionSelector = 1;
			self->callingSessionSelector = message->buffer[offset++] * 0x100;
			self->callingSessionSelector += message->buffer[offset++];*/
			break;
		case 52: /* Called Session Selector */
//			if (COTP_DEBUG) printf("iso_session: PI - Called Session Selector\r\n");
			if (parameterLength != 2)
				return SESSION_ERROR;
			hasCalledSessionSelector = 1;
			self->calledSessionSelector = message->buffer[offset++] * 0x100;
			self->calledSessionSelector += message->buffer[offset++];
			break;
		case 60: /* Data Overflow */
	/*		if (DEBUG) printf("iso_session: Parameter - Data Overflow\r\n");
			printf("TODO: parameter not implemented!");
			offset += parameterLength;*/
			break;
		case 193:	/* User Data */
//			if (COTP_DEBUG) printf("iso_session: PGI - user data\r\n");
			/* here we should return - the remaining data is for upper layers ! */

			if ((hasCalledSessionSelector) &&
				(hasConnectAcceptItem) &&
				(hasSessionRequirement))
			{
				//ByteBuffer* userData = calloc(1, sizeof(ByteBuffer));
				ByteBuffer_wrap(&self->userData, message->buffer + offset,
						message->size - offset, message->maxSize - offset);

				//self->userData = userData;

				return SESSION_OK;
			}
			else
				return SESSION_ERROR;
			break;
		case 194:	/* Extended User Data */
			//if (DEBUG) printf("iso_session: PGI - extended user data\r\n");
			break;
		default:
			//if (DEBUG) printf("iso_session: invalid parameter/PGI\r\n");
			break;
		}
	}

	return SESSION_ERROR;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static s32_t
  parseAcceptParameters( IsoSessPtr self, ByteBuffer* message,
                         s32_t startOffset, s32_t parameterLength ) {
/*----------------------------------------------------------------------------*/
	u8_t pi;
	u8_t param_len;
	u8_t param_val;
	u8_t hasProtocolOptions = 0;
	u8_t hasProtocolVersion = 0;
	s32_t offset = startOffset;
	s32_t maxOffset = offset + parameterLength;

	while (offset < maxOffset) {
		pi = message->buffer[offset++];
		param_len = message->buffer[offset++];

		switch (pi) {
		case 19: /* Protocol options */
			if (param_len != 1)
				return -1;
			self->protocolOptions = message->buffer[offset++];
//			if (COTP_DEBUG) printf("iso_session: Param - Protocol Options: %02x\r\n",
//				session->protocolOptions);
			hasProtocolOptions = 1;
			break;
		case 21: /* TSDU Maximum Size */
		/*	if (DEBUG) printf("iso_session: Param - TODO TSDU Maximum Size\r\n",
				param_val);
			offset += 4;*/
			break;
		case 22: /* Version Number */
			param_val = message->buffer[offset++];
//			if (COTP_DEBUG) printf("iso_session: Param - Version number: %02x\r\n",
//				param_val);
			if (param_val != 2)
				return -1;
			hasProtocolVersion = 1;
			break;
		case 23: /* Initial Serial Number */
		/*	if (DEBUG) printf("iso_session: Param - TODO Initial Serial Number\r\n");
			offset += param_len;*/
			break;
		case 26: /* Token Setting Item */
		/*	param_val = message->buffer[offset++];
			if (DEBUG) printf("iso_session: Param - Token Setting Item: %02x\r\n",
				param_val);*/
			break;
		case 55: /* Second Initial Serial Number */
		/*	if (DEBUG) printf("iso_session: Param - TODO Second Initial Serial
				Number\r\n");
			offset += param_len;*/
			break;
		case 56: /* Upper Limit Serial Number */
		/*	if (DEBUG) printf("iso_session: Param - TODO Upper Limit Serial
				Number\r\n");
			offset += param_len;*/
			break;
		case 57: /* Large Initial Serial Number */
		/*	if (DEBUG) printf("iso_session: Param - TODO Large Initial Serial
				Number\r\n");
			offset += param_len;*/
			break;
		case 58: /* Large Second Initial Serial Number */
		/*	if (DEBUG) printf("iso_session: Param - TODO Large Second Initial
				Serial Number\r\n");
			offset += param_len;*/
			break;
		default:
			//if (COTP_DEBUG)
			//	printf("iso_session: Param - Invalid Parameter with ID %02x\r\n", pi);
			break;
		}
	}

	if (hasProtocolOptions && hasProtocolVersion)
		return offset - startOffset;
	else
		return -1;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
void
  createSAcceptSpdu(IsoSessPtr self, SBuffer *sbuf) {
/*----------------------------------------------------------------------------*/
	s32_t offset = SBuffer_GetFrontSize(sbuf);
	u8_t* buf = SBuffer_GetFront(sbuf);
	s32_t lengthOffset;

	buf[offset++] = 14; /* ACCEPT SPDU */
	lengthOffset = offset; offset++;

	offset = encodeConnectAcceptItem(buf, offset, self->protocolOptions);

	offset = encodeSessionRequirement(self, buf, offset);

	offset = encodeCalledSessionSelector(self, buf, offset);

	offset = encodeSessionUserData(self, buf, offset, SBuffer_GetPayloadSize(sbuf));

	s32_t spduLength = (offset - lengthOffset - 1) + SBuffer_GetPayloadSize(sbuf);

	buf[lengthOffset] = spduLength;

	SBuffer_SetFrontSize(sbuf, offset);
	
	// т.к der_encode записывает в начало массива только заголовок session, то 
	// нужно помнить Расположение предыдущ.заголовков ppdu+acse+mms
	SBuffer_Shift(sbuf);
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static s32_t
  encodeConnectAcceptItem(u8_t *buf, s32_t offset, u8_t options) {
/*----------------------------------------------------------------------------*/
	buf[offset++] = 5;
	buf[offset++] = 6;
	buf[offset++] = 0x13; /* Protocol Options */
	buf[offset++] = 1;
	buf[offset++] = options;
	buf[offset++] = 0x16; /* Version Number */
	buf[offset++] = 1;
	buf[offset++] = 2;    /* Version = 2 */

	return offset;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static s32_t
  encodeSessionRequirement(IsoSessPtr self, u8_t *buf, s32_t offset) {
/*----------------------------------------------------------------------------*/
	buf[offset++] = 0x14;
	buf[offset++] = 2;
	buf[offset++] = (u8_t) (self->sessionRequirement / 0x100);
	buf[offset++] = (u8_t) (self->sessionRequirement & 0x00ff);

	return offset;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static s32_t
  encodeCalledSessionSelector(IsoSessPtr self, u8_t *buf, s32_t offset) {
/*----------------------------------------------------------------------------*/
	buf[offset++] = 0x34;
	buf[offset++] = 2;
	buf[offset++] = (u8_t) (self->calledSessionSelector / 0x100);
	buf[offset++] = (u8_t) (self->calledSessionSelector & 0x00ff);

	return offset;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static s32_t
  encodeSessionUserData(IsoSessPtr self, u8_t *buf, s32_t offset, u8_t payloadLength) {
/*----------------------------------------------------------------------------*/
	buf[offset++] = 0xc1;
	buf[offset++] = payloadLength;

	return offset;
}
