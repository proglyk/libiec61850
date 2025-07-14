#include "libiec61850/IsoServer.h"
#include "stdlib.h"

struct sIsoServer {
	IsoServerState 							state;
	ConnectionIndicationHandler connectionHandler;
	void* 											connectionHandlerParameter;
/* 	AcseAuthenticationParameter authParameter;
	Thread 											serverThread;
	ServerSocket2 							pxServSocket;
	sSocket2										xConnSocket;
	int 												tcpIp;*/
	IsoConnectionPtr 						pxConn;
};


/**	----------------------------------------------------------------------------
	* @brief ??? */
IsoServerPtr
	IsoServer_Create(s32_t socket) {
/*----------------------------------------------------------------------------*/
	IsoServerPtr self = calloc(1, sizeof(struct sIsoServer));
	self->state = ISO_SVR_STATE_IDLE;
  
  self->pxConn = IsoConnection_Create(socket);
  if (self->pxConn) return NULL;

	return self;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
void
	IsoServer_Delete(IsoServerPtr self) {
/*----------------------------------------------------------------------------*/
	if (self) {
		free(self);
		self = NULL;
	}
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
void
	IsoServer_ClientConnected(IsoServerPtr self) {
/*----------------------------------------------------------------------------*/
  IsoConnection_ClientConnected(self->pxConn);
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
void
	IsoServer_setConnectionHandler(IsoServerPtr self,
                                 ConnectionIndicationHandler handler,
                                 void* parameter) {
/*----------------------------------------------------------------------------*/
	self->connectionHandler = handler;
	self->connectionHandlerParameter = parameter;
}
