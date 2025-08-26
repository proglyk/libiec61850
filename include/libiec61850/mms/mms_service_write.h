/**
	******************************************************************************
  * @file    mms_service_write.h
  * @author  Michael Zillgith
  * @brief   This file is part of libIEC61850.
  *****************************************************************************/

#ifndef __MMS_SERVICE_WRITE_H__
#define __MMS_SERVICE_WRITE_H__

#include "libiec61850/mms/mms_server_conn.h"
#include "libiec61850/mms/mms_types.h"
#include "libiec61850/utils/byte_buffer.h"
#include "iso9506/WriteRequest.h"


/*
#include "server/mms_server_connection.h"
#include "server/byte_buffer.h"
#include "iso9506/ReadRequest.h"
#include "iso9506/AccessResult.h"
#include "ied/utility/linked_list.h"*/


/*******************************************************************************
	Globally viewed routines prototypes
*******************************************************************************/

int
	mmsServer_handleWriteRequest(MmsServerConnection* connection,	WriteRequest_t* 
		writeRequest, int invokeId, ByteBuffer* response);

#endif