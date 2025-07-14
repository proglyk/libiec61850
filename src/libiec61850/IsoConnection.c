#include "libiec61850/IsoConnection.h"
#include "stdlib.h"

struct sIsoConnection {
	uint8_t* receive_buf;
/*  	MessageReceivedHandler msgRcvdHandler;
	void* msgRcvdHandlerParameter; */
	s32_t socket;
/*int state;
	//CotpConnection * cotpConnection;
	//void* session;
  //void* presentation;
	//char* clientAddress;
	Resources xLayer; */
};

/**	----------------------------------------------------------------------------
	* @brief Connection init */
IsoConnectionPtr
	IsoConnection_Create(s32_t socket) {
/*----------------------------------------------------------------------------*/
	IsoConnectionPtr self = calloc(1, sizeof(struct sIsoConnection));
	self->socket = socket;
/*self->receive_buf = malloc(RECEIVE_BUF_SIZE);
	self->msgRcvdHandler = NULL;
	self->msgRcvdHandlerParameter = NULL;
	self->state = ISO_CON_STATE_STOPPED;
	SBuffer_Init(pxSbuf, p__buf1, SEND_BUF_SIZE); */
	return self;
}

/**	----------------------------------------------------------------------------
	* @brief Connection init */
void
	IsoConnection_Delete(IsoConnectionPtr self) {
/*----------------------------------------------------------------------------*/
	if (!self) return;
	if (self->receive_buf)
		free(self->receive_buf);
	free(self);
	self = NULL;
}

/**	----------------------------------------------------------------------------
	* @brief Connection init */
void
	IsoConnection_ClientConnected(IsoConnectionPtr self) {
/*----------------------------------------------------------------------------*/
}