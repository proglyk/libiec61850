#include "libiec61850_conf.h"
#include "libiec61850/IsoConnection.h"
#include "libiec61850/CotpConnection.h"
#include <stdlib.h>

// Type definitions

static void cotp_connect_indication(void);
static void cotp_data_indication(void);
static void session_connect(void);
static void session_data(void);

struct sIsoConnection {
  // Own needs
	uint8_t*               receive_buf;
  uint8_t*               send_buf;
  //MessageReceivedHandler msgRcvdHandler;
	//void*                  msgRcvdHandlerParameter;
	s32_t                  socket;
  int                    state;
  // Linkage with the upper layer
  CotpConnectionPtr      cotpConn;
  SBufferPtr             sbuf;
	
	//void* session;
  //void* presentation;
	//char* clientAddress;
	//Resources xLayer;

};

// Variables, Constants
static ByteBuffer receiveBuffer; // TODO разместить потом в куче

/**	----------------------------------------------------------------------------
	* @brief Iso Connection layer constructor */
IsoConnectionPtr
	IsoConnection_Create(s32_t socket) {
/*----------------------------------------------------------------------------*/
	// Self creating
  IsoConnectionPtr self = calloc(1, sizeof(struct sIsoConnection));
  if (!self) return NULL;
  // Self configurating
  self->socket = socket;
  self->receive_buf = malloc(RECEIVE_BUF_SIZE);
  self->send_buf = malloc(SEND_BUF_SIZE);
	//self->msgRcvdHandler = NULL;
	//self->msgRcvdHandlerParameter = NULL;
	self->state = ISO_CON_STATE_STOPPED;
  // создаем и линкуем SBuffer
  self->sbuf = calloc(1, sizeof(struct sSBuffer));
	SBuffer_Init(self->sbuf, self->send_buf, SEND_BUF_SIZE);
  // Top layers creating
  self->cotpConn = CotpConnection_Create(socket, &receiveBuffer, self->sbuf);
  if (!self->cotpConn) return NULL;
  
/*   self->xLayer.Cotp.px = calloc(1, sizeof(CotpConnection));
  self->xLayer.Session.px = calloc(1, sizeof(IsoSession));
  self->xLayer.Present.px = calloc(1, sizeof(IsoPresentation)); */
  
  self->state = ISO_CON_STATE_RUNNING;
  
	return self;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
s32_t
	IsoConnection_Init(IsoConnectionPtr self) {
/*----------------------------------------------------------------------------*/
/* 	CotpConnection_init(self->xLayer.Cotp.px, self->socket, &receiveBuffer);
  IsoSession_init(self->xLayer.Session.px);
  AcseConnection_init(&(self->xLayer.Acse.x));
	self->xLayer.Acse.x.authentication = NULL; */
  return 0;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
void
	IsoConnection_Deinit(IsoConnectionPtr self) {
/*----------------------------------------------------------------------------*/
/* 	AcseConnection_destroy(&(self->xLayer.Acse.x));
  CotpConnection_destroy(self->xLayer.Cotp.px); */
}

/**	----------------------------------------------------------------------------
	* @brief Connection init */
void
	IsoConnection_Delete(IsoConnectionPtr self) {
/*----------------------------------------------------------------------------*/
	if (!self) return;
/*   free(self->xLayer.Present.px);
	free(self->xLayer.Session.px);
  free(self->xLayer.Cotp.px); */
  if (self->cotpConn) CotpConnection_Delete(self->cotpConn);
	if (self->receive_buf) free(self->receive_buf);
  if (self->send_buf) free(self->send_buf);
  if (self->sbuf) free(self->sbuf);
	free(self);	self = NULL;
}

/**	----------------------------------------------------------------------------
	* @brief Connection init */
s32_t
	IsoConnection_ClientConnected(IsoConnectionPtr self) {
/*----------------------------------------------------------------------------*/
 //CotpIndication sta;
 s32_t rc;
 if (!self) goto exit;
 
 // read header
 //sta = CotpConnection_readHeaderTPKT(self->cotpConn);
 //if (sta != COTP_OK) goto exit;
 
 // buffers clear
 ByteBuffer_wrap(&receiveBuffer, self->receive_buf, 0, RECEIVE_BUF_SIZE);
 SBuffer_Clear(self->sbuf);
 
 // read body
 //sta = CotpConnection_parseIncomingMessage(self->cotpConn);
 rc = CotpConnection_Do(self->cotpConn);
  
  
  return 0;
  
  exit:
  self->state = ISO_CON_STATE_ERROR;
  return -1;
}

/**	----------------------------------------------------------------------------
	* @brief Connection init */
void
	IsoConnection_InstallListener( IsoConnectionPtr self,
                               MsgPassedHandlerPtr handler, void *param ) {
/*----------------------------------------------------------------------------*/
  CotpConnection_ThrowOverListener(self->cotpConn, handler, param);
}
