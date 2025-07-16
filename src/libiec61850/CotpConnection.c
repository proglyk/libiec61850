#include "libiec61850_conf.h"
#include "libiec61850/CotpConnection.h"
#include "libiec61850/IsoSession.h"

struct sCotpConnection {
  // локальные
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
  // связь с уровнями
  IsoSessionPtr isoSession;
};

static void CotpConnection_setTpduSize(CotpConnectionPtr, int);

/**	----------------------------------------------------------------------------
	* @brief Connection init */
CotpConnectionPtr
	CotpConnection_Create(s32_t socket, ByteBuffer* payloadBuffer) {
/*----------------------------------------------------------------------------*/
	CotpConnectionPtr self = calloc(1, sizeof(struct sCotpConnection));
  if (!self) return NULL;
	
  // Инит локальные перемен
  self->state = COTP_CON_STOP;
	self->socket = socket;
	self->srcRef = -1;
	self->dstRef = -1;
	self->class = -1;
	self->options = (CotpOptions) {.tpdu_size = 0, .tsap_id_src = -1, .tsap_id_dst = -1};
	self->payload = payloadBuffer;
  
  // Инит верхние уровни
  
	
	/* default TPDU size is maximum size */
	CotpConnection_setTpduSize(self, COTP_MAX_TPDU_SIZE);

	self->writeBuffer = NULL;
	//ByteBuffer_create(self->writeBuffer, 512);

//	self->stream = ByteStream_create(self->socket, self->writeBuffer);
  
  return self;
}

/**	----------------------------------------------------------------------------
	* @brief Connection init */
void
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