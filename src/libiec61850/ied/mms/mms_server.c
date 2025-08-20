//#include "mms_server.h"
#include "libiec61850/MmsServerConnection.h"
#include "libiec61850/ied/mms/mms_common.h"
#include "libiec61850/ied/mms/mms_server.h"


/**	----------------------------------------------------------------------------
	* @brief ??? */
MmsServer
MmsServer_create(void/* IsoServer isoServer, MmsDevice* device*/) {
/*----------------------------------------------------------------------------*/
	MmsServer self = calloc(1, sizeof(struct sMmsServer));

	//self->isoServer = isoServer;
	//self->device = device;
	//self->openConnections = Map_create();
	//self->valueCaches = createValueCachesForDomains(device);
	self->isLocked = false;
	//self->modelMutex = Semaphore_create(1);

	return self;
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

/**	----------------------------------------------------------------------------
	* @brief ??? */
void
  isoConnectionIndicationHandler( IsoConnIndication indication,
                                  void* parameter/*, IsoConnectionPtr conn*/) {
/*----------------------------------------------------------------------------*/
	MmsServer mmsServer = (MmsServer) parameter;
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
	}
}