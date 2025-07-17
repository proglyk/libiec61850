#include "libiec61850/IsoServer.h"
#include "stdlib.h"

// Type definitions

struct sIsoServer {
  // Own needs
	IsoServerState 							state;
	ConnectionIndicationHandler connectionHandler;
	void* 											connectionHandlerParameter;
/* 	AcseAuthenticationParameter authParameter;
	Thread 											serverThread;
	ServerSocket2 							pxServSocket;
	sSocket2										xConnSocket;
	int 												tcpIp;*/
  // Linkage with the upper layer
	IsoConnectionPtr 						isoConn;
};


/**	----------------------------------------------------------------------------
	* @brief Iso Server layer constructor */
IsoServerPtr
	IsoServer_Create(s32_t socket) {
/*----------------------------------------------------------------------------*/
	// Self creating
  IsoServerPtr self = calloc(1, sizeof(struct sIsoServer));
  if (!self) return NULL;
  // Self configurating
	self->state = ISO_SVR_STATE_IDLE; 
  // Top layers creating
  self->isoConn = IsoConnection_Create(socket, );
  if (self->isoConn) return NULL;

	return self;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
s32_t
	IsoServer_Init(void) {
/*----------------------------------------------------------------------------*/
	return 0;
}

/**	----------------------------------------------------------------------------
	* @brief Iso Server layer destructor */
void
IsoServer_Delete(IsoServerPtr self) {
/*----------------------------------------------------------------------------*/
	if (!self) return;
	if (self->isoConn) IsoConnection_Delete(self->isoConn);
	free(self); self = NULL;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
void
IsoServer_ClientConnected(IsoServerPtr self) {
/*----------------------------------------------------------------------------*/
  IsoConnection_ClientConnected(self->isoConn);
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
