#include "libiec61850_conf.h"
#include "libiec61850/CotpConnection.h"
#include "libiec61850/IsoSession.h"

// Type definitions

struct sCotpConnection {
  // Own needs
  CotpState state;
  s32_t socket;
 	int srcRef;
	int dstRef;
	int class;
	CotpOptions options;
	uint8_t eot;
	ByteBuffer* payload;
	ByteBuffer* writeBuffer;
//	ByteStream stream;
  // Linkage with the upper layer
  IsoSessionPtr isoSess;
};

static void CotpConnection_setTpduSize(CotpConnectionPtr, int);
static CotpIndication CotpConnection_readHeaderTPKT(CotpConnectionPtr);
static ByteBuffer *CotpConnection_getPayload(CotpConnectionPtr);

// Определения общедоступных (public) функций

/**	----------------------------------------------------------------------------
	* @brief Cotp Connection layer constructor */
CotpConnectionPtr
	CotpConnection_Create(s32_t socket, ByteBuffer* payloadBuffer) {
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
	
	/* default TPDU size is maximum size */
	CotpConnection_setTpduSize(self, COTP_MAX_TPDU_SIZE);

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
	* @brief Cotp Connection layer destructor */
static CotpIndication
  CotpConnection_readHeaderTPKT(CotpConnectionPtr self) {
/*----------------------------------------------------------------------------*/
	uint8_t local[2] = {0,0};

	if (ByteStream_read(self->stream, local, 2) < 0)
		return COTP_ERROR;
	if (local[0] != 0x03) return COTP_ERROR;
	if (local[1] != 0x00) return COTP_ERROR;

	return COTP_OK;
}

/**	----------------------------------------------------------------------------
	* @brief Cotp Connection layer action */
s32_t
  CotpConnection_Do(CotpConnectionPtr self) {
/*----------------------------------------------------------------------------*/
  CotpIndication sta;
  ByteBuffer* payload = NULL;
  
  // read header
  sta = CotpConnection_readHeaderTPKT(self->cotpConn);
  if (sta != COTP_OK) goto exit;
  
  // read body
  sta = CotpConnection_parseIncomingMessage(self->cotpConn);
  // см.протокол. Инициализация
  if (sta == COTP_CONNECT_INDICATION) {
    // ответ только для случая первоначального обращения
    if (self->state == COTP_CON_STOP) {
      CotpConnection_sendConnectionResponseMessage(self);
      self->state = COTP_CON_RUN;
    }
  }
  // Передача данных
  else if (sta == COTP_DATA_INDICATION) {
    if (self->state == COTP_CON_RUN) {
      payload = CotpConnection_getPayload(self);
      
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
	* @brief Connection init */
static void
  CotpConnection_setTpduSize(CotpConnectionPtr self, int tpduSize) {
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
	* @brief Connection init */
static ByteBuffer*
  CotpConnection_getPayload(CotpConnectionPtr self) {
/*----------------------------------------------------------------------------*/
	return self->payload;
}