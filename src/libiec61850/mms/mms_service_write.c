/**
	******************************************************************************
  * @file    mms_service_write.c
  * @author  Michael Zillgith
  * @brief   This file is part of libIEC61850.
  *****************************************************************************/

#include "libiec61850/mms/mms_service_write.h"
#include "libiec61850/mms/mms_server_common.h"
#include "libiec61850/mms/mms_common_internal.h"
#include "iso9506/WriteResponse.h"



/*******************************************************************************
	Locally viewed (static) routines prototypes
*******************************************************************************/

static int
	createMmsWriteResponse(MmsServerConnection* connection, int invokeId,
		ByteBuffer* response, MmsValueIndication indication);


/*******************************************************************************
	Globally viewed routines definitions
*******************************************************************************/


int
	mmsServer_handleWriteRequest(MmsServerConnection* connection,	WriteRequest_t* 
		writeRequest, int invokeId, ByteBuffer* response) {
			
	MmsValueIndication valueIndication;
	MmsValue* elementValue = NULL;
	AlternateAccess_t* alternateAccess;
	MmsValue* value;
	Data_t* dataElement;
	
	if (writeRequest->variableAccessSpecification.choice.listOfVariable.list.count != 1)
		return -1;

	ListOfVariableSeq_t* varSpec =
			writeRequest->variableAccessSpecification.choice.listOfVariable.list.array[0];

	if (varSpec->variableSpecification.present != VariableSpecification_PR_name) {
		createMmsWriteResponse(connection, invokeId, response, MMS_VALUE_ACCESS_DENIED);
		return 0;
	}

	if (varSpec->variableSpecification.choice.name.present != ObjectName_PR_domainspecific) {
		createMmsWriteResponse(connection, invokeId, response, MMS_VALUE_ACCESS_DENIED);
		return 0;
	}


	Identifier_t domainId = varSpec->variableSpecification.choice.name.choice.domainspecific.domainId;
	char* domainIdStr = createStringFromBuffer(domainId.buf, domainId.size);

	MmsDevice* device = MmsServer_getDevice(connection->server);

	MmsDomain* domain = MmsDevice_getDomain(device, domainIdStr);

	free(domainIdStr);

	if (domain == NULL) {
		createMmsWriteResponse(connection, invokeId, response, MMS_VALUE_ACCESS_DENIED);
		return 0;
	}

	Identifier_t nameId = varSpec->variableSpecification.choice.name.choice.domainspecific.itemId;
	char* nameIdStr = createStringFromBuffer(nameId.buf, nameId.size);

	MmsTypeSpecification* variable = MmsDomain_getNamedVariable(domain, nameIdStr);

	if (variable == NULL)
		goto return_access_denied;

	if (writeRequest->listOfData.list.count != 1)
		goto return_access_denied;

	alternateAccess = varSpec->alternateAccess;

	if (alternateAccess != NULL) {
		if (variable->type != MMS_ARRAY)
			goto return_access_denied;

		if (!mmsServer_isIndexAccess(alternateAccess))
			goto return_access_denied;
	}

	dataElement = writeRequest->listOfData.list.array[0];

	value = mmsMsg_parseDataElement(dataElement);

	if (value == NULL)
		goto return_access_denied;

	if (alternateAccess != NULL) {
		MmsValue* cachedArray = MmsServer_getValueFromCache(connection->server, domain, nameIdStr);

		if (cachedArray == NULL) {
			MmsValue_delete(value);
			goto return_access_denied;
		}

		int index = mmsServer_getLowIndex(alternateAccess);

		elementValue = MmsValue_getElement(cachedArray, index);

		if (elementValue == NULL) {
			MmsValue_delete(value);
			goto return_access_denied;
		}

		if (MmsValue_update(elementValue, value) == false) {
			MmsValue_delete(value);
			goto return_access_denied;
		}
	}

	MmsServer_lockModel(connection->server);
	
	valueIndication = MmsServer_setValue(connection->server, domain, nameIdStr, 
		value, connection);

	MmsServer_unlockModel(connection->server);

	createMmsWriteResponse(connection, invokeId, response, valueIndication);

	MmsValue_delete(value);

	free(nameIdStr);
	return 0;

return_access_denied:
	createMmsWriteResponse(connection, invokeId, response, MMS_VALUE_ACCESS_DENIED);
	free(nameIdStr);
	return 0;
}

/*******************************************************************************
	Locally viewed (static) routines definitions
*******************************************************************************/

	
static int
	createMmsWriteResponse(MmsServerConnection* connection, int invokeId, 
		ByteBuffer* response, MmsValueIndication indication) {
	
	MmsPdu_t* mmsPdu = mmsServer_createConfirmedResponse(invokeId);

	mmsPdu->choice.confirmedResponsePdu.confirmedServiceResponse.present =
			ConfirmedServiceResponse_PR_write;

	WriteResponse_t* writeResponse = &(mmsPdu->choice.confirmedResponsePdu.
		confirmedServiceResponse.choice.write);

	writeResponse->list.count = 1;
	writeResponse->list.size = 1;
	writeResponse->list.array = calloc(1, sizeof(WriteResponseList_t*));
	writeResponse->list.array[0] = calloc(1, sizeof(WriteResponseList_t));


	if (indication == MMS_VALUE_OK)
		writeResponse->list.array[0]->present = WriteResponseList_PR_success;
	else {
		writeResponse->list.array[0]->present = WriteResponseList_PR_failure;

		if (indication == MMS_VALUE_VALUE_INVALID)
			asn_long2INTEGER(&writeResponse->list.array[0]->choice.failure,
					DataAccessError_objectvalueinvalid);
		else if (indication == MMS_VALUE_ACCESS_DENIED)
			asn_long2INTEGER(&writeResponse->list.array[0]->choice.failure,
					DataAccessError_objectaccessdenied);
	}

	asn_enc_rval_t rval;

	rval = der_encode(&asn_DEF_MmsPdu, mmsPdu,
				mmsServer_write_out, (void*) response);

//	if (DEBUG) xer_fprint(stdout, &asn_DEF_MmsPdu, mmsPdu);

	asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);

 	return 0;
}