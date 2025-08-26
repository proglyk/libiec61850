/**
	******************************************************************************
  * @file    mms_service_read.h
  * @author  Michael Zillgith
  * @brief   This file is part of libIEC61850.
  *****************************************************************************/

#ifndef __MMS_SERVICE_READ_H__
#define __MMS_SERVICE_READ_H__


/*******************************************************************************
	Defines
*******************************************************************************/


#include "libiec61850/mms/mms_server_conn.h" //#include "server/mms_server_connection.h"
#include "libiec61850/utils/byte_buffer.h" //#include "server/byte_buffer.h"
#include "libiec61850/utils/linked_list.h"
#include "iso9506/ReadRequest.h"
#include "iso9506/AccessResult.h"



/*******************************************************************************
	Globally viewed routines prototypes
*******************************************************************************/

// typedef int MmsValue; //TODO
// void appendValueToResultList(MmsValue* value, LinkedList values,
	// AccessResult_t* accessResultList);


void
	mmsServer_handleReadRequest(MmsServerConnection* connection, ReadRequest_t* 
		read, int invokeId,	ByteBuffer* response);
		
// char*
	// createStringFromAsnIdentifier(Identifier_t identifier);

// typedef int MmsTypeSpecification; //TODO
// void
	// addNamedVariableToResultList(MmsTypeSpecification* namedVariable, MmsDomain* 
		// domain, char* nameIdStr, AccessResult_t* resultListEntry,	LinkedList values,
			// MmsServerConnection* connection, AlternateAccess_t* alternateAccess);


#endif /*__MMS_SERVICE_READ_H__*/