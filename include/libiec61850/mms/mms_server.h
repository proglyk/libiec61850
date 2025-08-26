/**
	******************************************************************************
  * @file    mms_server.h
  * @author  Michael Zillgith
  * @brief   This file is part of libIEC61850.
  *****************************************************************************/

#ifndef __MMS_SERVER_H__
#define __MMS_SERVER_H__


#include "libiec61850/mms/mms_device.h"
#include "libiec61850/api/map.h"
#include "libiec61850/IsoConnection.h"
#include "libiec61850/mms/mms_common.h"
#include <stdbool.h>

typedef MmsValue* (*ReadVariableHandler) (void* parameter, MmsDomain* domain, char*
	variableId);
	
typedef MmsValueIndication (*WriteVariableHandler)
		(void* parameter, MmsDomain* domain, char* variableId, MmsValue* value, 
		MmsServerConnection* connection);

typedef enum {
	MMS_SERVER_NEW_CONNECTION,
	MMS_SERVER_CONNECTION_CLOSED
} MmsServerEvent;
	
typedef void (*MmsConnectionHandler) (void* parameter, MmsServerConnection* connection, MmsServerEvent event);

typedef struct sMmsServer* MmsServer;
struct sMmsServer {
 	//IsoServerPtr isoServer;
	MmsDevice* device;
	ReadVariableHandler readHandler;
	void* readHandlerParameter;
	WriteVariableHandler writeHandler;
	void* writeHandlerParameter;
	MmsConnectionHandler connectionHandler;
	void* connectionHandlerParameter;
	Map openConnections;
	Map valueCaches;
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

MmsDevice* MmsServer_getDevice(MmsServer self);
MmsValue*	MmsServer_getValueFromCache(MmsServer self, MmsDomain* domain, char* itemId);
MmsValue*	MmsServer_getValue(MmsServer self, MmsDomain* domain, char* itemId);

void MmsServer_installReadHandler(MmsServer, ReadVariableHandler, void*);
void MmsServer_installWriteHandler(MmsServer, WriteVariableHandler, void*);
// A connection handler will be invoked whenever a new client connection is opened or closed
void MmsServer_installConnectionHandler(MmsServer, MmsConnectionHandler, void*);

void MmsServer_lockModel(MmsServer self);
void MmsServer_unlockModel(MmsServer self);
MmsValueIndication MmsServer_setValue(MmsServer, MmsDomain*, char*, MmsValue*, MmsServerConnection*);

/*void
	MmsServer_insertIntoCache(MmsServer self, MmsDomain* domain, char* itemId, 
		MmsValue* value);
		
void
	MmsServer_stopListening(MmsServer server);
	
void
MmsServer_startListening(MmsServer self, int tcpPort);

	
bool
	MmsServer_isLocked(MmsServer self);
	



		
#ifdef MMS_SERVER_HAS_VALUE_CACHE

MmsServer_enableValueCache(MmsServer);

#endif */

#endif /* __MMS_SERVER_H__ */
