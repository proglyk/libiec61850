/**
	******************************************************************************
  * @file    mms_service_named_variable_list.h
  * @author  Michael Zillgith
  * @brief   This file is part of libIEC61850.
  *****************************************************************************/

#ifndef __MMS_SERVICE_NAMED_VARIABLE_LIST_H__
#define __MMS_SERVICE_NAMED_VARIABLE_LIST_H__

//#include "server/mms_server_connection.h"
//#include "server/byte_buffer.h"
//#include "iso9506/GetNameListRequest.h"

//#define MMS_DATA_SET_SERVICE

#include "libiec61850/mms/mms_common.h"
//#include "server/mms_server_connection.h"
#include "libiec61850/utils/byte_buffer.h"
#include "iso9506/ConfirmedServiceRequest.h"


/*******************************************************************************
	Globally viewed routines prototypes
*******************************************************************************/

void
	mmsServer_handleGetNamedVariableListAttributesRequest( MmsServerConnection* 
		connection,	GetNamedVariableListAttributesRequest_t* request,	int invokeId,
		ByteBuffer* response);
//int
//	mmsServer_handleGetVariableAccessAttributesRequest( MmsServerConnection* 
//		connection, GetVariableAccessAttributesRequest_t* request, int invokeId,
//		ByteBuffer* response);
		
//void
//	mmsServer_handleDefineNamedVariableListRequest( MmsServerConnection* 
//		connection,	DefineNamedVariableListRequest_t* request, int invokeId, 
//		ByteBuffer* response);
		
//void
//	mmsServer_handleDeleteNamedVariableListRequest(MmsServerConnection* connection,
//		DeleteNamedVariableListRequest_t* request, int invokeId, ByteBuffer* 
//		response);

#endif /*__MMS_SERVICE_NAMED_VARIABLE_LIST_H__*/