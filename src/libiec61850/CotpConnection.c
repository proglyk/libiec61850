#include "libiec61850_conf.h"
#include "libiec61850/CotpConnection.h"
#include "libiec61850/IsoSession.h"
#include "libiec61850/SBuffer.h"

// Type definitions

struct sCotpConnection {
  // Own needs
  CotpState     state;
  s32_t         socket;
 	int           srcRef;
	int           dstRef;
	int           class;
	CotpOptions   options;
	uint8_t       eot;
	ByteBuffer*   payload;
	ByteBuffer*   writeBuffer;
//	ByteStream stream;
  // Linkage with the upper layer
  IsoSessionPtr isoSess;
  SBufferPtr    sbuf;
};

static CotpIndication	sendConnectionResponseMessage(CotpConnectionPtr);
static void setTpduSize(CotpConnectionPtr, int);
static CotpIndication readHeaderTPKT(CotpConnectionPtr);
static ByteBuffer *getPayload(CotpConnectionPtr);
// не проверял
static inline int getConnectionResponseLength(CotpConnectionPtr);
static CotpIndication parseIncomingMessage(CotpConnectionPtr);
static CotpIndication	sendSDataMessage(CotpConnectionPtr, SBuffer *);

// Определения общедоступных (public) функций

/**	----------------------------------------------------------------------------
	* @brief Cotp Connection layer constructor */
CotpConnectionPtr
	CotpConnection_Create(s32_t socket, ByteBuffer* payloadBuffer, SBufferPtr sbuf) {
/*----------------------------------------------------------------------------*/
	// Self creating
  CotpConnectionPtr self = calloc(1, sizeof(struct sCotpConnection));
  if (!self) return NULL;
  // Self configurating
  self->state = COTP_CON_STOP;
	self->socket = socket;
	self->srcRef = -1;
	self->dstRef = -1;
	self->class = -1;
	self->options = (CotpOptions) {.tpdu_size = 0, .tsap_id_src = -1, .tsap_id_dst = -1};
	self->payload = payloadBuffer;
  // Top layers creating
  self->isoSess = IsoSession_Create();
  if (!self->isoSess) return NULL;
  self->sbuf = sbuf;
	
	/* default TPDU size is maximum size */
	setTpduSize(self, COTP_MAX_TPDU_SIZE);

	self->writeBuffer = NULL;
	//ByteBuffer_create(self->writeBuffer, 512);

//	self->stream = ByteStream_create(self->socket, self->writeBuffer);
  
  return self;
}

/**	----------------------------------------------------------------------------
	* @brief Cotp Connection layer destructor */
void
  CotpConnection_Delete(CotpConnectionPtr self) {
/*----------------------------------------------------------------------------*/
	if (!self) return;
  if (self->isoSess) IsoSession_Delete(self->isoSess);
	free(self); self = NULL;
}

/**	----------------------------------------------------------------------------
	* @brief Cotp Connection layer action */
s32_t
  CotpConnection_Do(CotpConnectionPtr self) {
/*----------------------------------------------------------------------------*/
  CotpIndication sta;
  ByteBuffer* payload = NULL;
  s32_t rc;
  
  // read header
  sta = readHeaderTPKT(self);
  if (sta != COTP_OK) goto exit;
  
  // read body
  self->payload->size = 0; // INFO перешло из CotpConnection_parseIncomingMessage
  sta = parseIncomingMessage(self);
  // см.протокол. Инициализация
  if (sta == COTP_CONNECT_INDICATION) {
    // ответ только для случая первоначального обращения
    if (self->state == COTP_CON_STOP) {
      sendConnectionResponseMessage(self);
      self->state = COTP_CON_RUN;
    }
  }
  // Передача данных
  else if (sta == COTP_DATA_INDICATION) {
    if (self->state == COTP_CON_RUN) {
      payload = getPayload(self);
      rc = IsoSession_Do(self->isoSess, payload);
      if (rc < 0) goto exit;
      sta = sendSDataMessage(self, pxSbuf);
      if (sta != COTP_OK) goto exit;
    }
  }
  // Exception
  else {
    goto exit;
  }
  
  return 0;
  
  exit:
  self->state = COTP_CON_STOP;
  return -1;
}

// Определения локальных (private) функций

/**	----------------------------------------------------------------------------
	* @brief ??? */
static CotpIndication
	sendSDataMessage(CotpConnectionPtr self, SBuffer *payload) {
/*----------------------------------------------------------------------------*/
	fragments = 1;
	currentBufPos = 0;

	fragmentPayloadSize = CotpConnection_getTpduSize(self) - COTP_DATA_HEADER_SIZE;

	
	// Is segmentation required?
	if (SBuffer_GetPayloadSize(payload) > fragmentPayloadSize) { 
		fragments = SBuffer_GetPayloadSize(payload) / fragmentPayloadSize;

		if ((SBuffer_GetPayloadSize(payload) % fragmentPayloadSize) != 0) {
			fragments += 1;
		}
	}

	while (fragments > 0) {
		if (fragments > 1) {
			currentLimit = currentBufPos + fragmentPayloadSize;
			lastUnit = 0;
		}
		else {
			currentLimit = SBuffer_GetPayloadSize(payload);
			lastUnit = 1;
		}

		if (writeRfc1006Header(self, 7 + (currentLimit - currentBufPos)) == COTP_ERROR)
				return COTP_ERROR;

		if (writeDataTpduHeader(self, lastUnit) == COTP_ERROR)
			return COTP_ERROR;

		int i;
		for (i = currentBufPos; i < currentLimit; i++) {
			if (ByteStream_writeUint8(self->stream, SBuffer_GetValue(payload,i)) != 1)
				return COTP_ERROR;
			currentBufPos++;
		}

		ByteStream_sendBuffer(self->stream);
		fragments--;
	}

	return COTP_OK;
}

static CotpIndication
parseIncomingMessage(CotpConnectionPtr self) {
	uint8_t len;
	uint8_t tpduType;
	uint16_t rfc1006Length;
  
	if (ByteStream_readUint16(self->stream, &rfc1006Length) == -1)
		return COTP_ERROR;

	if (ByteStream_readUint8(self->stream, &len) != 1)
		return COTP_ERROR;


	if (ByteStream_readUint8(self->stream, &tpduType) != 1)
		return COTP_ERROR;

	switch (tpduType) {
		case 0xe0:
//			LWIP_DEBUGF(COTP_DEBUG, ("TPDU Code: 0xe0 - CR\r\n"));
			if (cotp_parse_CRequest_tpdu(self, len) == 1)
				return COTP_CONNECT_INDICATION;
			else
				return COTP_ERROR;
		case 0xd0:
			/*self->eot = 1;
			if (cotp_parse_CConfirm_tpdu(self, len) == 1)
				return COTP_CONNECT_INDICATION;
			else
				return COTP_ERROR;*/
		case 0xf0:
//			LWIP_DEBUGF(COTP_DEBUG, ("TPDU Code: 0xf0 - DT\r\n"));
			if (cotp_parse_DATA_tpdu(self, len) == 1) {
				if (addPayloadToBuffer(self, rfc1006Length) == 1){
					//LWIP_DEBUGF(COTP_DEBUG, ("TPDU Payload - OK\r\n"));
					return COTP_DATA_INDICATION;
				}	else {
					//LWIP_DEBUGF(COTP_DEBUG, ("TPDU Payload - ERR\r\n"));
					return COTP_ERROR;
				}
			}
			else
				return COTP_ERROR;
		default:
			return COTP_ERROR;
	}
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
CotpIndication
  sendConnectionResponseMessage(CotpConnectionPtr self) {
/*----------------------------------------------------------------------------*/
	allocateWriteBuffer(self);
	
	int len = getConnectionResponseLength(self);

	if (writeRfc1006Header(self, len) == COTP_ERROR)
		return COTP_ERROR;

	if (writeStaticConnectResponseHeader(self) != COTP_OK)
		return COTP_ERROR;

	if (writeOptions(self) != COTP_OK)
		return COTP_ERROR;

	p__cotp = self;

//	if (COTP_DEBUG) printf("Message for responce=:%s\r\n", self->stream->writeBuffer->buffer);


	if (ByteStream_sendBuffer(self->stream) == -1) {
		printf("Error sending buffer\r\n");
		return COTP_ERROR;
	}

	return COTP_OK;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static CotpIndication
  readHeaderTPKT(CotpConnectionPtr self) {
/*----------------------------------------------------------------------------*/
	uint8_t local[2] = {0,0};

	if (ByteStream_read(self->stream, local, 2) < 0)
		return COTP_ERROR;
	if (local[0] != 0x03) return COTP_ERROR;
	if (local[1] != 0x00) return COTP_ERROR;

	return COTP_OK;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static void
  setTpduSize(CotpConnectionPtr self, int tpduSize) {
/*----------------------------------------------------------------------------*/
  int newTpduSize = 1;

  if (tpduSize > COTP_MAX_TPDU_SIZE)
    tpduSize = COTP_MAX_TPDU_SIZE;

  while ((1 << newTpduSize) < tpduSize)
    newTpduSize++;

  if ((1 << newTpduSize) > tpduSize)
    newTpduSize--;

  self->options.tpdu_size = newTpduSize;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static ByteBuffer*
  getPayload(CotpConnectionPtr self) {
/*----------------------------------------------------------------------------*/
	return self->payload;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static inline int
  getConnectionResponseLength(CotpConnectionPtr self) {
/*----------------------------------------------------------------------------*/
	return 11 + getOptionsLength(self);
}
