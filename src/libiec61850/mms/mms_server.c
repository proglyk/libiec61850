/**
	******************************************************************************
  * @file    mms_server.c
  * @author  Ilia Proniashin
  * @version V1.0.0
  * @date    26-August-2025
  * @brief   Implementation of industrial standard IEC61850 for ARM Cortex-M
  *   devices runned under RTOS
  *****************************************************************************/
  
//#include "mms_server.h"
//#include "libiec61850/mms/mms_server_conn.h"
#include "libiec61850/mms/mms_common.h"
#include "libiec61850/mms/mms_server.h"
#include "libiec61850/mms/mms_value_cache.h"
#include "libiec61850/mms/mms_value.h"

static Map createValueCachesForDomains(MmsDevice* device);


/**	----------------------------------------------------------------------------
	* @brief ??? */
MmsServer
MmsServer_create(/*void IsoServer isoServer,*/ MmsDevice* device) {
/*----------------------------------------------------------------------------*/
	MmsServer self = calloc(1, sizeof(struct sMmsServer));

	//self->isoServer = isoServer;
	self->device = device;
	//self->openConnections = Map_create();
	self->valueCaches = createValueCachesForDomains(device);
	self->isLocked = false;
	//self->modelMutex = Semaphore_create(1);

	return self;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
void
  MmsServer_Init(MmsServer self) {
/*----------------------------------------------------------------------------*/
  //MmsServerConnection* mmsCon;
  
  //mmsCon = MmsServerConnection_init(0, self);
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
  //MmsServerConnection* mmsCon;
  
  //MmsServerConnection* mmsCon = (MmsServerConnection*) Map_removeEntry(mmsServer->openConnections, conn, false);
  // if (mmsServer->connectionHandler != NULL)
      // mmsServer->connectionHandler(mmsServer->connectionHandlerParameter,
        // mmsCon, MMS_SERVER_CONNECTION_CLOSED);
  //if (mmsCon != NULL)	MmsServerConnection_destroy(mmsCon);
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
	* @brief MMS server conn layer destructor */
void
  MmsServer_conn_opened(MmsServer self, void *pld) {
/*----------------------------------------------------------------------------*/
  if (self->connHandler != NULL) {
    self->connHandler( self->connParam, pld, MMS_SERVER_NEW_CONNECTION );
  }
}

/**	----------------------------------------------------------------------------
	* @brief MMS server conn layer destructor */
void
  MmsServer_conn_closed(MmsServer self, void *pld) {
/*----------------------------------------------------------------------------*/
  if (self->connHandler != NULL) {
    self->connHandler( self->connParam, pld, MMS_SERVER_CONNECTION_CLOSED );
  }
}

///////// ВПЛЕТЕНЫ В КОД ..

/**	----------------------------------------------------------------------------
	* @brief Функция обновления полей подписок
			CHANGE _V_0_3_0_MYSELF
	* @param dataset: Номер текущего датасета посылки.
	* @param cargpage: Номер текущей страницы каретки.
	* @param ptr: Указатель на структуру со значениями из посылки.
	* @retval none: Нет */
MmsValueIndication
MmsServer_setValue(MmsServer self, MmsDomain* domain, char* itemId, MmsValue* value,
	MmsServerConnection* connection) {
/*----------------------------------------------------------------------------*/
	MmsValueIndication indication = MMS_VALUE_ACCESS_DENIED;
	// 
	MmsValue* cachedValue = MmsServer_getValueFromCache(self, domain, itemId);
	//
	if (cachedValue != NULL) {
		MmsValue_update(cachedValue, value); // проверить на возврат.знач
		indication = MMS_VALUE_OK;
	} else {
		if (self->writeHandler != NULL) {
			indication = self->writeHandler(self->writeHandlerParameter, domain, itemId, value, connection);
		}
	}
	
	return indication;
}

MmsDevice*	MmsServer_getDevice(MmsServer self) {
	return self->device;
}

MmsValue*
MmsServer_getValue(MmsServer self, MmsDomain* domain, char* itemId)
{
	MmsValue* value = NULL;

	value = MmsServer_getValueFromCache(self, domain, itemId);

	if (value == NULL)
		if (self->readHandler != NULL) {
			//LWIP_DEBUGF(ISO9506_DEBUG, ("MmsServer_getValue: ...\r\n"));
			value = self->readHandler(self->readHandlerParameter, domain, itemId);
		}

	return value;
}

MmsValue*
MmsServer_getValueFromCache(MmsServer self, MmsDomain* domain, char* itemId)
{
	MmsValueCache cache = Map_getEntry(self->valueCaches, domain);

	if (cache != NULL) {
		return MmsValueCache_lookupValue(cache, itemId);
	}

	return NULL;
}

/**	----------------------------------------------------------------------------
	* @brief связывает MmsMapping */
void
  MmsServer_installReadHandler( MmsServer self,
                                ReadVariableHandler handler,
                                void* parameter) {
/*----------------------------------------------------------------------------*/
	self->readHandler = handler;
	self->readHandlerParameter = parameter;
}

/**	----------------------------------------------------------------------------
	* @brief связывает MmsMapping */
void
  MmsServer_installWriteHandler( MmsServer self,
                                 WriteVariableHandler handler,
                                 void* parameter) {
/*----------------------------------------------------------------------------*/
	self->writeHandler = handler;
	self->writeHandlerParameter = parameter;
}

// /**	----------------------------------------------------------------------------
	// * @brief связывает MmsMapping */
// void
  // MmsServer_installConnectionHandler( MmsServer self, 
                                      // MmsConnectionHandler handler,
                                      // void* parameter) {
// /*----------------------------------------------------------------------------*/
	// self->connectionHandler = handler;
	// self->connectionHandlerParameter = parameter;
// }

/**	----------------------------------------------------------------------------
	* @brief связывает MmsMapping */
void
  MmsServer_installConnHandler( MmsServer self, MmsConnHandler handler,
                                void *param) {
/*----------------------------------------------------------------------------*/
	self->connHandler = handler;
	self->connParam = param;
}

void MmsServer_lockModel(MmsServer self) {
	//Semaphore_wait(self->modelMutex);
}


void MmsServer_unlockModel(MmsServer self) {
	//Semaphore_post(self->modelMutex);
}

void
MmsServer_insertIntoCache(MmsServer self, MmsDomain* domain, char* itemId, MmsValue* value)
{
	MmsValueCache cache = Map_getEntry(self->valueCaches, domain);

	if (cache != NULL) {
		MmsValueCache_insertValue(cache, itemId, value);
	}
}

//CHANGE _V_0_3_0_
void
MmsServer_startListening(MmsServer server, int tcpPort)
{
	// пока не исп. закоменчиваем
	//IsoServer_setConnectionHandler(server->isoServer, isoConnectionIndicationHandler, (void*) server);
	//IsoServer_setTcpPort(server->isoServer, tcpPort);
	//IsoServer_startListening(server->isoServer);
}

void
MmsServer_stopListening(MmsServer server)
{
	// пока не исп. закоменчиваем
	//IsoServer_stopListening(server->isoServer);
}

static Map
	createValueCachesForDomains(MmsDevice* device) {
	
	Map valueCaches =  Map_create();

	int i;
	for (i = 0; i < device->domainCount; i++) {
		MmsValueCache valueCache = MmsValueCache_create(device->domains[i]);
		Map_addEntry(valueCaches, device->domains[i], valueCache);
	}

	return valueCaches;
}
