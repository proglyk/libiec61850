#include "libiec61850/IsoPresentation.h"
#include "libiec61850/AcseConnection.h"
#include "libiec61850/utils/byte_buffer.h"
#include "libiec61850/iso8823/CPType.h"
#include <string.h>

// Type definitions

struct sIsoPresentation {
  // Own needs
	u32_t             callingPresentationSelector;
	u32_t             calledPresentationSelector;
	CtxDef            context[2];
	u8_t              nextContextId;
  // Layer buffer
	ByteBuffer        nextPayload;
  // Linkage with the upper layer
  AcseConnectionPtr acseConn;
  SBufferPtr        sbuf;
  MsgPassedHandlerPtr msgPassedHandler;
  void               *msgPassedParam;
};

static IsoPresStatus parseConnectPdu(IsoPresPtr, ByteBuffer *);
static IsoPresStatus parseUserData(IsoPresPtr, ByteBuffer *);
static IsoPresStatus createSUserData(IsoPresPtr, SBuffer *);
static int calcLengthOfBERLengthField(int value);
// blindly copied from origin
static IsoPresStatus setContextDefinition(IsoPresPtr, int, ContextListSeq_t *);
static int parseBERLengthField(uint8_t *, int, int *);
static int encodeBERLengthField(uint8_t *, int, int);

/**	----------------------------------------------------------------------------
	* @brief Iso Presentation layer constructor */
IsoPresPtr
	IsoPresentation_Create(SBufferPtr sbuf) {
/*----------------------------------------------------------------------------*/
	// Self creating
  IsoPresPtr self = calloc(1, sizeof(struct sIsoPresentation));
  if (!self) return NULL;
  // линкуем SBuffer
  self->sbuf = sbuf;
  // Top layers creating
  self->acseConn = AcseConnection_Create(self->sbuf);
  if (!self->acseConn) return NULL;
  
  return self;
}

/**	----------------------------------------------------------------------------
	* @brief Iso Presentation layer destructor */
void
  IsoPresentation_Delete(IsoPresPtr self) {
/*----------------------------------------------------------------------------*/
	if (!self) return;
  if (self->acseConn) AcseConnection_Delete(self->acseConn);
	free(self); self = NULL;
}

/**	----------------------------------------------------------------------------
	* @brief Connection init */
void
	IsoPresentation_InstallListener( IsoPresPtr self, MsgPassedHandlerPtr handler,
                                   void* param ) {
/*----------------------------------------------------------------------------*/
	self->msgPassedHandler = handler;
	self->msgPassedParam = param;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
void
	IsoPresentation_ThrowOverListener( IsoPresPtr self, MsgPassedHandlerPtr handler,
                                     void *param ) {
/*----------------------------------------------------------------------------*/
  AcseConnection_InstallListener(self->acseConn, handler, param);
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
s32_t
  IsoPresentation_Connect(IsoPresPtr self, ByteBuffer *buffer) {
/*----------------------------------------------------------------------------*/
	// ???
  IsoPresStatus sta = parseConnectPdu(self, buffer);
  if (sta != PRESENTATION_OK) return -1;
  //
  // TODO AcseConnection_parseMessage(self->acseConn, self->nextPayload);
  return 0;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
s32_t
  IsoPresentation_ProcessData(IsoPresPtr self, ByteBuffer *buffer) {
/*----------------------------------------------------------------------------*/
  if (!self->msgPassedHandler) return -1;
  // ???
  IsoPresStatus sta = parseUserData(self, buffer);
  if (sta != PRESENTATION_OK) return -1;
  
  if ( self->nextContextId == 3 ) {
    ByteBuffer *mmsConfRequest = &( self->nextPayload );
    self->msgPassedHandler( self->msgPassedParam, mmsConfRequest, self->sbuf );
    createSUserData(self, self->sbuf);
  }
  
  return 0;
}

// Определения локальных (private) функций

/**	----------------------------------------------------------------------------
	* @brief ??? */
static IsoPresStatus
  parseConnectPdu(IsoPresPtr self, ByteBuffer *buffer) {
/*----------------------------------------------------------------------------*/
	CPType_t* cptype = 0;
	asn_dec_rval_t rval;
  int pdvListEntries;
  int decodedOctets;
	int dataListEntries;
	int userDataSize;

  rval = ber_decode(NULL, &asn_DEF_CPType, (void**)&cptype,
    ByteBuffer_getBuffer(buffer), ByteBuffer_getSize(buffer));
  if (rval.code != RC_OK){
    //if (ISO8823_DEBUG) printf("iso_presentation.c: broken CPType encoding at "
    //  "byte %ld\r\n",	(long) rval.consumed);
		goto error;
  }

	decodedOctets = rval.consumed;
	if (cptype->modeSelector.modeValue != (long)modeValue_normalmode) {
		//if (ISO8823_DEBUG) printf("iso_presentation.c: only normal mode"
    //  "supported!");
		goto error;
	}
//  if (ISO8823_DEBUG) printf("modeSelector:= %02x\r\n",
//  	cptype->modeSelector.modeValue);

	pdvListEntries =
    cptype->normalModeParameters->contextDefinitionList->list.count;
	if (pdvListEntries != 2) {
		//if (ISO8823_DEBUG) printf("iso_presentation: 2 pdv list items required"
    //  " found: %i\r\n", pdvListEntries);
		goto error;
	}
//  if (ISO8823_DEBUG) printf("pdvListEntries:= %02d\r\n", pdvListEntries);


	int i;
	for (i = 0; i < pdvListEntries; i++) {
		ContextListSeq_t* member =
				cptype->normalModeParameters->contextDefinitionList->list.array[i];

		if (setContextDefinition(self, i, member) == PRESENTATION_ERROR)
			goto error;
	}

	if (cptype->normalModeParameters->userData->present ==
    Userdata_PR_fullyencodeddata){
		dataListEntries = cptype->normalModeParameters->userData->choice.
      fullyencodeddata.list.count;
		PDVlist_t* pdvListEntry = cptype->normalModeParameters->userData->choice.
      fullyencodeddata.list.array[0];
		userDataSize = pdvListEntry->presentationdatavalues.choice.singleASN1type.
      size;
		uint8_t* userData = pdvListEntry->presentationdatavalues.choice.
      singleASN1type.buf;
		self->nextContextId = (uint8_t)pdvListEntry->presentationcontextidentifier;

		ByteBuffer_wrap(&(self->nextPayload),buffer->buffer + (decodedOctets -
      userDataSize), userDataSize, userDataSize);
	} else {
		//if (ISO8823_DEBUG) printf("iso_presentation.c: Unsupported user data: "
    //  "simply encoded data\r\n");
		goto error;
	}

//	if (ISO8823_DEBUG) printf("dataListEntries:= %02d\r\n",
//    dataListEntries);
//	if (ISO8823_DEBUG) printf("nextContextId:= %02d\r\n",
//    self->nextContextId);

  asn_DEF_CPType.free_struct(&asn_DEF_CPType, cptype, 0);
  return PRESENTATION_OK;
  error:
  asn_DEF_CPType.free_struct(&asn_DEF_CPType, cptype, 0);
  return PRESENTATION_ERROR;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static IsoPresStatus
  parseUserData(IsoPresPtr self, ByteBuffer *buffer) {
/*----------------------------------------------------------------------------*/
 	int length = buffer->size;
	uint8_t* buf = buffer->buffer;

	if (length < 9)
		return PRESENTATION_ERROR;

	if (buf[0] != 0x61)
		return PRESENTATION_ERROR;

	int pos = 1;
	pos = parseBERLengthField(buf, pos, NULL);

	if (buf[pos++] != 0x30)
		return PRESENTATION_ERROR;

	pos = parseBERLengthField(buf, pos, NULL);

	if (buf[pos++] != 0x02)
		return PRESENTATION_ERROR;

	if (buf[pos++] != 0x01)
		return PRESENTATION_ERROR;

	self->nextContextId = buf[pos++];

	if (buf[pos++] != 0xa0)
		return PRESENTATION_ERROR;

	int userDataLength;

	pos = parseBERLengthField(buf, pos, &userDataLength);

	ByteBuffer_wrap(&(self->nextPayload), buf + pos, userDataLength, userDataLength);

	return PRESENTATION_OK;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static IsoPresStatus
	createSUserData(IsoPresPtr self, SBuffer *sbuf) {
/*----------------------------------------------------------------------------*/
	//int lenght =  ByteBuffer_getSize(writeBuffer);
	int lenght = 0;
	//uint8_t* buf = ByteBuffer_getBuffer(writeBuffer);
	uint8_t* buf = SBuffer_GetFront(sbuf);

	//int payloadLength = ByteBuffer_getSize(payload);
	int payloadLength = SBuffer_GetPayloadSize(sbuf);

	int userDataLengthFieldSize = calcLengthOfBERLengthField(payloadLength);
	int pdvListLength = payloadLength + (userDataLengthFieldSize + 4);

	int pdvListLengthFieldSize = calcLengthOfBERLengthField(pdvListLength);
	int presentationLength = pdvListLength + (pdvListLengthFieldSize + 1);

	// int presentationLengthFieldSize = calcLengthOfBERLengthField(totalLength);
	buf[lenght++] = (uint8_t) 0x61;
	lenght = encodeBERLengthField(buf, lenght, presentationLength);
	buf[lenght++] = (uint8_t) 0x30;
	lenght = encodeBERLengthField(buf, lenght, pdvListLength);
	buf[lenght++] = (uint8_t) 0x02;
	buf[lenght++] = (uint8_t) 0x01;
	buf[lenght++] = (uint8_t) self->nextContextId;
	buf[lenght++] = (uint8_t) 0xa0;
	lenght = encodeBERLengthField(buf, lenght, payloadLength);

	SBuffer_SetFrontSize(sbuf, lenght);
	SBuffer_Shift(sbuf);
	//ByteBuffer_setSize(writeBuffer, lenght);

	//ByteBuffer_append(writeBuffer, ByteBuffer_getBuffer(payload), payloadLength);

	return PRESENTATION_OK;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static int
  calcLengthOfBERLengthField(int value) {
/*----------------------------------------------------------------------------*/
	if (value < 128)
		return 1;
	if (value < 0x100)
		return 2;
	if (value < 0x10000) {
		return 3;
	}
	return -1;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static IsoPresStatus
  setContextDefinition( IsoPresPtr self, int index,
                                ContextListSeq_t *member ) {
/*----------------------------------------------------------------------------*/
	if (member->abstractsyntaxname.size > 9) {
		//if (ISO8823_DEBUG) printf("iso_presentation.c: ASN OID size to large.\r\n");
		return PRESENTATION_ERROR;
	}

	self->context[index].contextId =
			(uint8_t)member->presentationcontextidentifier;

	self->context[index].abstractSyntaxName[0] =
			 member->abstractsyntaxname.size;

	memcpy(self->context[index].abstractSyntaxName + 1,
			member->abstractsyntaxname.buf, member->abstractsyntaxname.size);

	return PRESENTATION_OK;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static int
  parseBERLengthField(uint8_t* buffer, int pos, int* length) {
/*----------------------------------------------------------------------------*/
	int len;

	if (buffer[pos] & 0x80)
		len = buffer[pos] & 0x7f;
	else
		len = 0;

	if (length != NULL) {
		if (len == 0)
			*length = buffer[pos];
		else if (len == 1) {
			*length = buffer[pos + 1];
		}
		else {
			*length = (buffer[pos + 1] * 0x100) + buffer[pos + 2];
		}
	}

	return pos + 1 + len;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static int
  encodeBERLengthField(uint8_t* buffer, int pos, int value) {
/*----------------------------------------------------------------------------*/
	if (value < 128) {
		buffer[pos++] = (uint8_t) value;
		return pos;
	}
	if (value < 0x100) {
		buffer[pos++] = (uint8_t) 0x81;
		buffer[pos++] = (uint8_t) value;
		return pos;
	}
	if (value < 0x10000) {
		buffer[pos++] = (uint8_t) 0x82;
		buffer[pos++] = (uint8_t) (value / 0x100);
		buffer[pos++] = (uint8_t) (value & 0xff);
		return pos;
	}
	return -1;
}
