#include "libiec61850_conf.h"
#include "libiec61850/IsoConnection.h"
#include "libiec61850/CotpConnection.h"
#include "stdlib.h"


static void cotp_connect_indication(void);
static void cotp_data_indication(void);
static void session_connect(void);
static void session_data(void);

struct sIsoConnection {
  // локальные
	uint8_t*               receive_buf;
  uint8_t*               send_buf;
  MessageReceivedHandler msgRcvdHandler;
	void*                  msgRcvdHandlerParameter;
	s32_t                  socket;
  int                    state;
  // Top layers linkage
  CotpConnectionPtr      cotpConn;
  
	
	//void* session;
  //void* presentation;
	//char* clientAddress;
	//Resources xLayer;
  SBuffer xSbuf;
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
	self->msgRcvdHandler = NULL;
	self->msgRcvdHandlerParameter = NULL;
	self->state = ISO_CON_STATE_STOPPED;
	SBuffer_Init(&self->xSbuf, self->send_buf, SEND_BUF_SIZE);
  // Top layers creating
  self->cotpConn = CotpConnection_Create(socket, &receiveBuffer);
  if (!self->cotpConn) return NULL;
  
/*   self->xLayer.Cotp.px = calloc(1, sizeof(CotpConnection));
  self->xLayer.Session.px = calloc(1, sizeof(IsoSession));
  self->xLayer.Present.px = calloc(1, sizeof(IsoPresentation)); */
  
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
	if (self->receive_buf) free(self->receive_buf);
  if (self->send_buf) free(self->send_buf);
	free(self);	self = NULL;
}

/**	----------------------------------------------------------------------------
	* @brief Connection init */
void
	IsoConnection_installListener( IsoConnectionPtr self,
                                 MessageReceivedHandler handler,
                                 void* parameter )
{
/*----------------------------------------------------------------------------*/
	self->msgRcvdHandler = handler;
	self->msgRcvdHandlerParameter = parameter;
}

/**	----------------------------------------------------------------------------
	* @brief Connection init */
s32_t
	IsoConnection_ClientConnected(IsoConnectionPtr self) {
/*----------------------------------------------------------------------------*/
 
  return 0;
  
  exit:
  return -1;
}

