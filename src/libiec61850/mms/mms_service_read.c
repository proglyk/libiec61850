/**
	******************************************************************************
  * @file    mms_service_read.c
  * @author  Michael Zillgith
  * @brief   This file is part of libIEC61850.
  *****************************************************************************/

#include "libiec61850/mms/mms_service_read.h"
#include "libiec61850/mms/mms_server_common.h"
#include "libiec61850/mms/mms_server.h"
#include "libiec61850/mms/mms_value.h"
#include "libiec61850/mms/mms_common_internal.h"


/*******************************************************************************
	Locally viewed (static) routines prototypes
*******************************************************************************/

static void
	addComplexValueToResultList(AccessResult_t* accessResult, 
		MmsTypeSpecification* namedVariable, LinkedList typedValues, 
			MmsServerConnection* connection, MmsDomain* domain, char* nameIdStr);

static void
	handleReadListOfVariablesRequest(	MmsServerConnection* connection, 
		ReadRequest_t* read, int invokeId, ByteBuffer* response);
		
static void
	handleReadNamedVariableListRequest(	MmsServerConnection* connection,
		ReadRequest_t* read, int invokeId, ByteBuffer* response);
		
static MmsPdu_t*
	createReadResponse(int invokeId);

//static AccessResult_t**
//	createAccessResultsList(MmsPdu_t* mmsPdu, int resultsCount);
	
static bool
	isSpecWithResult(ReadRequest_t* read);
	
//static void
//	addResultToResultList(AccessResult_t* accessResult, MmsValue* value);
	
void
alternateArrayAccess(MmsServerConnection* connection,
		AlternateAccess_t* alternateAccess, MmsDomain* domain,
		char* itemId, LinkedList values, AccessResult_t* accessResult,
		MmsTypeSpecification* namedVariable);
			
static void
	singleValueAccess(LinkedList values, 	MmsServerConnection* connection,
		MmsDomain* domain, char* itemId,
			AccessResult_t* accessResult);
			
static Data_t*
	addNamedVariableValue(MmsTypeSpecification* namedVariable, MmsServerConnection* connection,
		MmsDomain* domain, char* itemId, LinkedList typedValues);
		
static void
	deleteValueList(LinkedList values);
	
static void
	freeMmsReadResponsePdu(MmsPdu_t* mmsPdu);
	
static void
	createNamedVariableListResponse(MmsServerConnection* connection, 
		MmsNamedVariableList namedList,	int invokeId, ByteBuffer* response, 
			ReadRequest_t* read);

static bool
isAccessToArrayComponent(AlternateAccess_t* alternateAccess);


static MmsValue*
getComponentOfArrayElement(AlternateAccess_t* alternateAccess, MmsTypeSpecification* namedVariable,
        MmsValue* structuredValue);


/*******************************************************************************
	Locally viewed variables
*******************************************************************************/

static char buf[32] = {0};


/*******************************************************************************
	Globally viewed routines definitions
*******************************************************************************/

void
	appendValueToResultList(MmsValue* value, LinkedList values,	AccessResult_t* 
		accessResultList) {

	if (value != NULL )
		LinkedList_add(values, value);

	mmsMsg_addResultToResultList(accessResultList, value);
}


void
	mmsServer_handleReadRequest(MmsServerConnection* connection, ReadRequest_t* 
		read,	int invokeId,	ByteBuffer* response) {
		
	if (read->variableAccessSpecification.present == 
		VariableAccessSpecification_PR_listOfVariable) {
		handleReadListOfVariablesRequest(connection, read, invokeId, response);
	}
#if (MMS_DATA_SET_SERVICE == 1)
	else if (read->variableAccessSpecification.present == 
		VariableAccessSpecification_PR_variableListName) {
		handleReadNamedVariableListRequest(connection, read, invokeId, response);
	}
#endif /*(MMS_DATA_SET_SERVICE == 1)*/
	else {
		mmsServer_createConfirmedErrorPdu(invokeId, response, 
			MMS_ERROR_TYPE_OBJECT_ACCESS_UNSUPPORTED);
	}
}


void
addNamedVariableToResultList(MmsTypeSpecification* namedVariable, MmsDomain* domain, char* nameIdStr, AccessResult_t* resultListEntry,
		LinkedList /*<MmsValue>*/ values, MmsServerConnection* connection, AlternateAccess_t* alternateAccess)
{
	if (namedVariable != NULL) {

		//if (IEDREAD_DEBUG) printf("addNamedVariableToResultList: found named variable %s with search string %s\r\n",
		//		namedVariable->name, nameIdStr);

		if (namedVariable->type == MMS_STRUCTURE) {
			//if (IEDREAD_DEBUG) printf("  type == MMS_STRUCTURE\r\n");
			MmsValue* value = MmsServer_getValue(connection->server, domain, nameIdStr);

		  if (value != NULL) {
				appendValueToResultList(value, values, resultListEntry);
		  } else {
				addComplexValueToResultList(resultListEntry, namedVariable, values, 
					connection, domain, nameIdStr);
			}
		}
		else if (namedVariable->type == MMS_ARRAY) {
			//if (IEDREAD_DEBUG) printf("  type == MMS_ARRAY\r\n");
			if (alternateAccess != NULL) {
				alternateArrayAccess(connection, alternateAccess, domain,
						nameIdStr, values, resultListEntry, namedVariable);
			}
			else { //getCompleteArray
				singleValueAccess(values, connection, domain, nameIdStr,
						resultListEntry);
			}
		}
		else {
			//if (IEDREAD_DEBUG) printf("  type == ELSE\r\n");
			singleValueAccess(values, connection, domain, nameIdStr,
					resultListEntry);
		}

	}
	else {
		mmsMsg_addResultToResultList(resultListEntry, NULL);
	}
}


//CHANGE _VER_0_3_2_
void
alternateArrayAccess(MmsServerConnection* connection,
		AlternateAccess_t* alternateAccess, MmsDomain* domain,
		char* itemId, LinkedList values, AccessResult_t* accessResult,
		MmsTypeSpecification* namedVariable)
{
	if (mmsServer_isIndexAccess(alternateAccess))
	{
		int lowIndex = mmsServer_getLowIndex(alternateAccess);
		int numberOfElements = mmsServer_getNumberOfElements(alternateAccess);

		//if (IEDREAD_DEBUG) printf("Alternate access index: %i elements %i\r\n",
		//		lowIndex, numberOfElements);

		int index = lowIndex;

		MmsValue* value = NULL;

		MmsValue* arrayValue = MmsServer_getValue(connection->server, domain, itemId);

		if (arrayValue != NULL) {

			if (numberOfElements == 0)
			    if (isAccessToArrayComponent(alternateAccess)) {
			        if (namedVariable->typeSpec.array.elementTypeSpec->type == MMS_STRUCTURE) {
			            MmsValue* structValue = MmsValue_getElement(arrayValue, index);

			            if (structValue != NULL)
			                value = getComponentOfArrayElement(alternateAccess,
			                        namedVariable, structValue);
			        }
			    }
			    else
			        value = MmsValue_getElement(arrayValue, index);
			else {
				value = MmsValue_createEmtpyArray(numberOfElements);

				MmsValue_setDeletable(value);

				int resultIndex = 0;
				while (index < lowIndex + numberOfElements) {
					MmsValue* elementValue = NULL;

					elementValue = MmsValue_getElement(arrayValue, index);

					if (!MmsValue_isDeletable(elementValue))
						elementValue = MmsValue_clone(elementValue);

					MmsValue_setArrayElement(value, resultIndex, elementValue);

					index++;
					resultIndex++;
				}
			}

			appendValueToResultList(value, values, accessResult);

			if (MmsValue_isDeletable(arrayValue))
				MmsValue_delete(arrayValue);

		}
		else
			mmsMsg_addResultToResultList(accessResult, NULL);



	}
	else { // invalid access
		//if (IEDREAD_DEBUG) printf("Invalid alternate access\r\n");
		mmsMsg_addResultToResultList(accessResult, NULL);
	}
}


/*******************************************************************************
	Locally viewed (static) routines definitions
*******************************************************************************/


static void
	addComplexValueToResultList(AccessResult_t* accessResult, 
		MmsTypeSpecification* namedVariable, LinkedList typedValues, 
			MmsServerConnection* connection, MmsDomain* domain, char* nameIdStr) {
	
	int componentCount = namedVariable->typeSpec.structure.elementCount;

	accessResult->present = AccessResult_PR_structure;
	accessResult->choice.structure.list.count = componentCount;
	accessResult->choice.structure.list.size = componentCount;

	accessResult->choice.structure.list.array = calloc(componentCount, sizeof(Data_t*));

	int i;
	for (i = 0; i < componentCount; i++) {
		char* newNameIdStr = createString(3, nameIdStr, "$",
				namedVariable->typeSpec.structure.elements[i]->name);
		accessResult->choice.structure.list.array[i] =
				addNamedVariableValue(namedVariable->typeSpec.structure.elements[i],
						connection, domain, newNameIdStr,typedValues);
		//if (IEDREAD_DEBUG) printf("addComplexValueToResultList: ...\r\n");
		free(newNameIdStr);
	}
}


static void
	handleReadListOfVariablesRequest(	MmsServerConnection* connection, 
		ReadRequest_t* read, int invokeId, ByteBuffer* response) {

	int variableCount = read->variableAccessSpecification.choice.listOfVariable.list.count;

	MmsPdu_t* mmsPdu = createReadResponse(invokeId);

	AccessResult_t** accessResultList = mmsMsg_createAccessResultsList(mmsPdu, 
		variableCount);

	LinkedList /*<MmsValue>*/ values = LinkedList_create();

	if (isSpecWithResult(read)) { /* add specification to result */
		mmsPdu->choice.confirmedResponsePdu.confirmedServiceResponse.choice.read.variableAccessSpecification
			= &(read->variableAccessSpecification);
	}

	MmsServer_lockModel(connection->server);

	int i;

	for (i = 0; i < variableCount; i++) {
		AccessResult_t* resultListEntry = accessResultList[i];

		VariableSpecification_t varSpec =
			read->variableAccessSpecification.choice.listOfVariable.list.array[i]->variableSpecification;

		AlternateAccess_t* alternateAccess =
			read->variableAccessSpecification.choice.listOfVariable.list.array[i]->alternateAccess;

		if (varSpec.present == VariableSpecification_PR_name) {

			if (varSpec.choice.name.present == ObjectName_PR_domainspecific) {
				char* domainIdStr = mmsMsg_createStringFromAsnIdentifier(
						varSpec.choice.name.choice.domainspecific.domainId);

				char* nameIdStr = mmsMsg_createStringFromAsnIdentifier(
						varSpec.choice.name.choice.domainspecific.itemId);

				MmsDomain* domain = MmsDevice_getDomain(MmsServer_getDevice(connection->server), domainIdStr);

				if (domain == NULL) {
					//if (IEDREAD_DEBUG) printf("MMS read: domain %s not found!\r\n", domainIdStr);
					mmsMsg_addResultToResultList(resultListEntry, NULL);
					break;
				}

				MmsTypeSpecification* namedVariable = MmsDomain_getNamedVariable(domain, nameIdStr);

				addNamedVariableToResultList(namedVariable, domain, nameIdStr, resultListEntry,
						values, connection, alternateAccess);

				free(domainIdStr);
				free(nameIdStr);
			}
			else {
				//TODO should we send a ConfirmedErrorPdu here?
				mmsMsg_addResultToResultList(resultListEntry, NULL);
				//if (IEDREAD_DEBUG) printf("MMS read: object name type not supported!\r\n");
			}
		}
		else {
			//TODO should we send a ConfirmedErrorPdu here?
			mmsMsg_addResultToResultList(resultListEntry, NULL);
			//if (IEDREAD_DEBUG) printf("MMS read: varspec type not supported!\r\n");
		}
	}
	
	if (invokeId == 5) {
		asm("nop");
	}
	if (invokeId == 7) {
		asm("nop");
	}

	MmsServer_unlockModel(connection->server);

	asn_enc_rval_t rval =
		der_encode_to_buffer(&asn_DEF_MmsPdu, mmsPdu, response->buffer, connection->maxPduSize);

	deleteValueList(values);

	mmsPdu->choice.confirmedResponsePdu.confirmedServiceResponse.choice.read.variableAccessSpecification
		= NULL;

	freeMmsReadResponsePdu(mmsPdu);
	
	/* Check if response is to large for maximum PDU size */
	if (rval.encoded != -1) {
		response->size = rval.encoded;
	}
	else {
		printf("Error encoding MMS PDU max PDU size of %i exceeded at type (%s)\r\n", connection->maxPduSize, rval.failed_type->name);
		response->size = 0;
		mmsServer_createConfirmedErrorPdu(invokeId, response,
				MMS_ERROR_TYPE_RESPONSE_EXCEEDS_MAX_PDU_SIZE);
	}
}


static void
handleReadNamedVariableListRequest(
		MmsServerConnection* connection,
		ReadRequest_t* read,
		int invokeId,
		ByteBuffer* response)
{
	if (read->variableAccessSpecification.choice.variableListName.present ==
			ObjectName_PR_domainspecific)
	{
		char* domainId = mmsMsg_createStringFromAsnIdentifier(
				read->variableAccessSpecification.choice.variableListName.choice.domainspecific.domainId);

		char* listName = mmsMsg_createStringFromAsnIdentifier(
				read->variableAccessSpecification.choice.variableListName.choice.domainspecific.itemId);

		MmsDomain* domain = MmsDevice_getDomain(MmsServer_getDevice(connection->server), domainId);

		if (domain == NULL) {
			//if (IEDREAD_DEBUG) printf("MMS read: domain %s not found!\r\n", domainId);
			mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_TYPE_OBJECT_NON_EXISTENT);
		}
		else {
			MmsNamedVariableList namedList = MmsDomain_getNamedVariableList(domain, listName);

			if (namedList != NULL) {
				createNamedVariableListResponse(connection, namedList, invokeId, response, read);
			}
			else {
				//if (IEDREAD_DEBUG) printf("MMS read: named variable list %s not found!\r\n", listName);
				mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_TYPE_OBJECT_NON_EXISTENT);
			}
		}

		free(domainId);
		free(listName);
	} else if (read->variableAccessSpecification.choice.variableListName.present ==
				ObjectName_PR_aaspecific) {
		char* listName = mmsMsg_createStringFromAsnIdentifier(read->variableAccessSpecification.
				choice.variableListName.choice.aaspecific);

		MmsNamedVariableList namedList = MmsServerConnection_getNamedVariableList(connection, listName);

		if (namedList == NULL)
			mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_TYPE_OBJECT_NON_EXISTENT);
		else {
			createNamedVariableListResponse(connection, namedList, invokeId, response, read);
		}

		free(listName);
	}	else
		mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_TYPE_OBJECT_ACCESS_UNSUPPORTED);
}


static MmsPdu_t*
createReadResponse(int invokeId)
{
	MmsPdu_t* mmsPdu = mmsServer_createConfirmedResponse(invokeId);

	mmsPdu->choice.confirmedResponsePdu.confirmedServiceResponse.present =
				ConfirmedServiceResponse_PR_read;

	ReadResponse_t* readResponse =
			&(mmsPdu->choice.confirmedResponsePdu.confirmedServiceResponse.choice.read);

	readResponse->variableAccessSpecification = NULL;

	return mmsPdu;
}





static bool
isSpecWithResult(ReadRequest_t* read)
{
	if (read->specificationWithResult != NULL)
		if (*(read->specificationWithResult) != false)
			return true;

	return false;
}





static void //TODO remove me
	singleValueAccess(LinkedList values, 	MmsServerConnection* connection,
		MmsDomain* domain, char* itemId, AccessResult_t* accessResult) {
			
	MmsValue* value = MmsServer_getValue(connection->server, domain, itemId);
	appendValueToResultList(value, values, accessResult);
}


//CHANGE _V_0_3_0_
static Data_t*
	addNamedVariableValue(MmsTypeSpecification* namedVariable, MmsServerConnection* connection,
		MmsDomain* domain, char* itemId, LinkedList typedValues) {
	
	Data_t* dataElement = NULL;
	MmsValue* value = NULL;
	
	if (!namedVariable) {
		asm("nop");
	}

	if (namedVariable->type == MMS_STRUCTURE) {
		dataElement = calloc(1, sizeof(Data_t));

		int componentCount = namedVariable->typeSpec.structure.elementCount;
		dataElement->present = Data_PR_structure;
		dataElement->choice.structure = calloc(1, sizeof(DataSequence_t));

		dataElement->choice.structure->list.size = componentCount;
		dataElement->choice.structure->list.count = componentCount;
		dataElement->choice.structure->list.array = calloc(componentCount, sizeof(Data_t*));

		int i;

		for (i = 0; i < componentCount; i++) {
			char* newNameIdStr = createString(3, itemId, "$",
					namedVariable->typeSpec.structure.elements[i]->name);
			
			if (!strcmp("LLN01$GO$Control1$NdsCom", newNameIdStr)){
				asm("nop");
			}

			dataElement->choice.structure->list.array[i] =
					addNamedVariableValue(namedVariable->typeSpec.structure.elements[i],
							connection, domain,	newNameIdStr, typedValues);
			
			free(newNameIdStr);
		}
	}
	else {
		value = MmsServer_getValue(connection->server, domain, itemId);

		//TODO add checks???

		if (value != NULL) {
			LinkedList_add(typedValues, value);
			dataElement = mmsMsg_createBasicDataElement(value);
			printf("addNamedVariableValue %s\r\n", itemId);
		}
		else {
			//TODO remove printf
			printf("Error getting element %s\r\n", itemId);
		}
		
		value += 0;
	}

	return dataElement;
}


static void
deleteValueList(LinkedList values)
{
	LinkedList value = values;
	MmsValue* typedValue;
	while ((value = LinkedList_getNext(value)) != NULL ) {
		typedValue = (MmsValue*) (value->data);

		if (typedValue->deleteValue == 1) {
			MmsValue_delete(typedValue);
		}
	}
	LinkedList_destroyStatic(values);
}


static void
freeMmsReadResponsePdu(MmsPdu_t* mmsPdu)
{
	int accessResultSize =
			mmsPdu->choice.confirmedResponsePdu.confirmedServiceResponse.choice.read.listOfAccessResult.list.count;

	AccessResult_t** accessResult =
			mmsPdu->choice.confirmedResponsePdu.confirmedServiceResponse.choice.read.listOfAccessResult.list.array;

	mmsMsg_deleteAccessResultList(accessResult, accessResultSize);

	mmsPdu->choice.confirmedResponsePdu.confirmedServiceResponse.choice.read.listOfAccessResult.list.array = 0;
	mmsPdu->choice.confirmedResponsePdu.confirmedServiceResponse.choice.read.listOfAccessResult.list.count = 0;
	mmsPdu->choice.confirmedResponsePdu.confirmedServiceResponse.choice.read.listOfAccessResult.list.size =	0;

	asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);
}


static void
	createNamedVariableListResponse(MmsServerConnection* connection, MmsNamedVariableList namedList,
		int invokeId, ByteBuffer* response, ReadRequest_t* read) {

	MmsPdu_t* mmsPdu = createReadResponse(invokeId);

	LinkedList /*<MmsValue>*/ values = LinkedList_create();
	LinkedList variables = MmsNamedVariableList_getVariableList(namedList);

	int variableCount = LinkedList_size(variables);

	AccessResult_t** accessResultList =
		mmsMsg_createAccessResultsList(mmsPdu, variableCount);

	if (isSpecWithResult(read)) { /* add specification to result */
		mmsPdu->choice.confirmedResponsePdu.confirmedServiceResponse.choice.read.variableAccessSpecification
			= &(read->variableAccessSpecification);
	}

	MmsServer_lockModel(connection->server);

	int i;

	LinkedList variable = LinkedList_getNext(variables);

	for (i = 0; i < variableCount; i++) {
		AccessResult_t* resultListEntry = accessResultList[i];
		//char* variableName = (char*)

		MmsNamedVariableListEntry variableListEntry = (MmsNamedVariableListEntry) variable->data;

		MmsDomain* variableDomain = MmsNamedVariableListEntry_getDomain(variableListEntry);
		char* variableName = MmsNamedVariableListEntry_getVariableName(variableListEntry);

		MmsTypeSpecification* namedVariable = MmsDomain_getNamedVariable(variableDomain,
				variableName);

		addNamedVariableToResultList(namedVariable, variableDomain, variableName, resultListEntry,
								values, connection, NULL);

		variable = LinkedList_getNext(variable);
	}

	MmsServer_unlockModel(connection->server);

	asn_enc_rval_t rval =
		der_encode_to_buffer(&asn_DEF_MmsPdu, mmsPdu, response->buffer, connection->maxPduSize);

	deleteValueList(values);

	mmsPdu->choice.confirmedResponsePdu.confirmedServiceResponse.choice.read.variableAccessSpecification
			= NULL;

	freeMmsReadResponsePdu(mmsPdu);
	
	/* Check if response is to large for maximum PDU size */
	if (rval.encoded != -1) {
		response->size = rval.encoded;
	}
	else {
		response->size = 0;
		mmsServer_createConfirmedErrorPdu(invokeId, response,
				MMS_ERROR_TYPE_RESPONSE_EXCEEDS_MAX_PDU_SIZE);
	}
}


//CHANGE _VER_0_3_2_
static bool
isAccessToArrayComponent(AlternateAccess_t* alternateAccess)
{
	/*
    if (alternateAccess->list.array[0]->choice.unnamed.choice.selectAlternateAccess.
                alternateAccess != NULL)
        return true;
    else
        return false;
	*/
	return false;
}


//CHANGE _VER_0_3_2_
static MmsValue*
getComponentOfArrayElement(AlternateAccess_t* alternateAccess, MmsTypeSpecification* namedVariable,
        MmsValue* structuredValue)
{
    //alternateAccess->list.
		Identifier_t component = {NULL, 0};

    if (isAccessToArrayComponent(alternateAccess))
    {
				// Несоответствие AlternateAccess
        //Identifier_t component = alternateAccess->list.array[0]->choice.unnamed.choice.selectAlternateAccess.alternateAccess
        //        ->list.array[0]->choice.unnamed->choice.selectAccess.choice.component;

        if (component.size > 129)
            return NULL;

        int elementCount = namedVariable->typeSpec.structure.elementCount;


        MmsTypeSpecification* structSpec = namedVariable->typeSpec.array.elementTypeSpec;

        int i;
        for (i = 0; i < elementCount; i++) {
            if (strncmp (structSpec->typeSpec.structure.elements[i]->name, 
							(char const*)component.buf, component.size) == 0) {
                MmsValue* value = MmsValue_getStructElementByIndex(structuredValue, i);
                return value;
            }
        }
    }

    return NULL;
}



