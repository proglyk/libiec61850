/**
	******************************************************************************
  * @file    ???
  * @author  Ilia Proniashin
  * @version V1.0.0
  * @date    26-August-2025
  * @brief   Implementation of industrial standard IEC61850 for ARM Cortex-M
  *   devices runned under RTOS
  *****************************************************************************/
  
//#include "mms_server.h"
#include "libiec61850/mms/mms_server_conn.h"
#include "libiec61850/mms/mms_common.h"
#include "libiec61850/mms/mms_server.h"


/**	----------------------------------------------------------------------------
	* @brief ??? */
MmsServer
MmsServer_create(/*void IsoServer isoServer,*/ MmsDevice* device) {
/*----------------------------------------------------------------------------*/
	MmsServer self = calloc(1, sizeof(struct sMmsServer));

	//self->isoServer = isoServer;
	self->device = device;
	//self->openConnections = Map_create();
	//self->valueCaches = createValueCachesForDomains(device);
	self->isLocked = false;
	//self->modelMutex = Semaphore_create(1);

	return self;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
void
  MmsServer_Init(MmsServer self) {
/*----------------------------------------------------------------------------*/
  MmsServerConnection* mmsCon;
  
  mmsCon = MmsServerConnection_init(0, self);
  //Map_addEntry(self->openConnections, conn, mmsCon);
  // if (self->connectionHandler != NULL)
    // self->connectionHandler(self->connectionHandlerParameter,
        // mmsCon, MMS_SERVER_NEW_CONNECTION);
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
void
  MmsServer_Deinit(MmsServer self) {
/*----------------------------------------------------------------------------*/
  MmsServerConnection* mmsCon;
  
  //MmsServerConnection* mmsCon = (MmsServerConnection*) Map_removeEntry(mmsServer->openConnections, conn, false);
  // if (mmsServer->connectionHandler != NULL)
      // mmsServer->connectionHandler(mmsServer->connectionHandlerParameter,
        // mmsCon, MMS_SERVER_CONNECTION_CLOSED);
  if (mmsCon != NULL)	MmsServerConnection_destroy(mmsCon);
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
void
MmsServer_destroy(MmsServer self) {
/*----------------------------------------------------------------------------*/
	//Map_deleteDeep(self->openConnections, false, closeConnection);
	//Map_deleteDeep(self->valueCaches, false, (void(*)(void*))deleteSingleCache);
	//Semaphore_destroy(self->modelMutex);
	free(self);
}

static void isoConnectionIndicationHandler( IsoConnIndication, void* );
/**	----------------------------------------------------------------------------
	* @brief ??? */
void
  isoConnectionIndicationHandler( IsoConnIndication indication,
                                  void* parameter/*, IsoConnectionPtr conn*/) {
/*----------------------------------------------------------------------------*/
/* 	MmsServer mmsServer = (MmsServer) parameter;
	MmsServerConnection* mmsCon;

	if (indication == ISO_CONNECTION_OPENED) {
		mmsCon = MmsServerConnection_init(0, mmsServer);
		//Map_addEntry(mmsServer->openConnections, conn, mmsCon);
		if (mmsServer->connectionHandler != NULL)
			mmsServer->connectionHandler(mmsServer->connectionHandlerParameter,
					mmsCon, MMS_SERVER_NEW_CONNECTION);
	} else {
		//MmsServerConnection* mmsCon = (MmsServerConnection*) Map_removeEntry(mmsServer->openConnections, conn, false);
		if (mmsServer->connectionHandler != NULL)
				mmsServer->connectionHandler(mmsServer->connectionHandlerParameter,
					mmsCon, MMS_SERVER_CONNECTION_CLOSED);
		if (mmsCon != NULL)	MmsServerConnection_destroy(mmsCon);
	} */
}

// Заглушки, требуют реализации
MmsDevice*	MmsServer_getDevice(MmsServer self) {
	return self->device;
}