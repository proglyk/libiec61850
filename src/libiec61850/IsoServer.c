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
	int 												tcpIp;
	int 												tcpPort;
	IsoConnection 							pxIsoConnection; */
};


/**	----------------------------------------------------------------------------
	* @brief ??? */
IsoServerPtr
	IsoServer_create(void) {
/*----------------------------------------------------------------------------*/
	// с занулёнными байтами
	IsoServerPtr self = calloc(1, sizeof(struct sIsoServer));
	
	self->state = ISO_SVR_STATE_IDLE;
	//self->tcpPort = 102;

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
	IsoServer_setConnectionHandler(IsoServerPtr self,
                                 ConnectionIndicationHandler handler,
                                 void* parameter) {
/*----------------------------------------------------------------------------*/
	self->connectionHandler = handler;
	self->connectionHandlerParameter = parameter;
}
