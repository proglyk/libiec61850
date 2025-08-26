/**
	******************************************************************************
  * @file    ???
  * @author  Ilia Proniashin
  * @version V1.0.0
  * @date    26-August-2025
  * @brief   Implementation of industrial standard IEC61850 for ARM Cortex-M
  *   devices runned under RTOS
  *****************************************************************************/
  
#ifndef __MMS_SERVER_H__
#define __MMS_SERVER_H__


#include "libiec61850/mms/mms_device.h"
//#include "ied/map.h"
#include "libiec61850/IsoConnection.h"
#include "libiec61850/mms/mms_common.h"
#include <stdbool.h>

// typedef MmsValue* (*ReadVariableHandler) (void* parameter, MmsDomain* domain, char*
	// variableId);
	
// typedef MmsValueIndication (*WriteVariableHandler)
		// (void* parameter, MmsDomain* domain, char* variableId, MmsValue* value, 
		// MmsServerConnection* connection);

typedef enum {
	MMS_SERVER_NEW_CONNECTION,
	MMS_SERVER_CONNECTION_CLOSED
} MmsServerEvent;
	
typedef void (*MmsConnectionHandler) (void* parameter, MmsServerConnection* connection, MmsServerEvent event);

typedef struct sMmsServer* MmsServer;
struct sMmsServer {
 	//IsoServerPtr isoServer;
	MmsDevice* device;
	/*ReadVariableHandler readHandler;
	void* readHandlerParameter;
	WriteVariableHandler writeHandler;
	void* writeHandlerParameter;*/
	MmsConnectionHandler connectionHandler;
	void* connectionHandlerParameter;
	/*Map openConnections;
	Map valueCaches;*/
	bool isLocked;
	//Semaphore modelMutex;
};


struct sMmsServerConnection {
	int maxServOutstandingCalling;
	int maxServOutstandingCalled;
	int dataStructureNestingLevel;
	int maxPduSize; // local detail
	//IsoConnection isoConnection;
	MmsServer server;
  // aa-specific named variable lists
	LinkedList namedVariableLists;
};

MmsServer	MmsServer_create(/* void IsoServerPtr,  */MmsDevice *);
void      MmsServer_destroy(MmsServer);
void	    MmsServer_Init( MmsServer );
void	    MmsServer_Deinit( MmsServer );


// далее заглушки

MmsDevice*	MmsServer_getDevice(MmsServer self);

/*void
	MmsServer_insertIntoCache(MmsServer self, MmsDomain* domain, char* itemId, 
		MmsValue* value);
		
void
	MmsServer_stopListening(MmsServer server);
	
void
MmsServer_startListening(MmsServer self, int tcpPort);
	
void
	MmsServer_lockModel(MmsServer self);
	
void
	MmsServer_unlockModel(MmsServer self);
	
MmsValue*
	MmsServer_getValue(MmsServer self, MmsDomain* domain, char* itemId);
	
bool
	MmsServer_isLocked(MmsServer self);
	
MmsValue*
	MmsServer_getValueFromCache(MmsServer self, MmsDomain* domain, char* itemId);

MmsValueIndication
MmsServer_setValue(MmsServer self, MmsDomain* domain, char* itemId, MmsValue* value, MmsServerConnection* connection);
	
void
	MmsServer_installReadHandler(MmsServer self, ReadVariableHandler, void* 
		parameter);

void
	MmsServer_installWriteHandler(MmsServer self, WriteVariableHandler, void* 
		parameter);

// A connection handler will be invoked whenever a new client connection is opened or closed
MmsServer_installConnectionHandler(MmsServer self, MmsConnectionHandler, void*
  parameter);
		
#ifdef MMS_SERVER_HAS_VALUE_CACHE

MmsServer_enableValueCache(MmsServer);

#endif */

#endif /* __MMS_SERVER_H__ */
