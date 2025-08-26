/**
	******************************************************************************
  * @file    mms_service_get_namelist.c
  * @author  Michael Zillgith
  * @brief   This file is part of libIEC61850.
  *****************************************************************************/

//#include "ied/mms/mms_get_namelist_service.h"
#include "libiec61850/mms/mms_server_conn.h"
#include "iso9506/GetNameListRequest.h"
#include "libiec61850/utils/string_utilities.h"
#include "libiec61850/utils/linked_list.h"
#include "libiec61850/mms/mms_device.h"
#include "libiec61850/mms/mms_server_common.h"
#include "stack_config.h"
#include "libiec61850/utils/ber_encoder.h"
#include "libiec61850/mms/mms_device.h"

/*
#include "server/mms_get_namelist_service.h"
#include "server/string_utilities.h"
#include "server/linked_list.h"
#include "server/mms_server_common.h"
*/


/*******************************************************************************
	Locally viewed (static) routines prototypes
*******************************************************************************/

static LinkedList
	getNameListDomainSpecific(MmsServerConnection* connection, char* domainName);
	
static void
createNameListResponse(MmsServerConnection* connection, int invokeId, LinkedList
	nameList, ByteBuffer* response, char* continueAfter);
		
static LinkedList
	getNameListVMDSpecific(MmsServerConnection* connection);
	
static LinkedList
addSubNamedVaribleNamesToList(LinkedList nameList, char* prefix, 
	MmsTypeSpecification* variable);
		
static LinkedList
	createStringsFromNamedVariableList(LinkedList nameList, LinkedList 
		variableLists);
		
static LinkedList
	getNamedVariableListDomainSpecific(MmsServerConnection* connection, char* 
		domainName);
		
static LinkedList
	getNamedVariableListAssociationSpecific(MmsServerConnection* connection);
	
static char* appendMmsSubVariable(char* name, char* child);
	

/*******************************************************************************
	Locally viewed variables
*******************************************************************************/

static uint8_t * p_objectClass;
static long objectClass;
static long objectScope;
static GetNameListRequest_t* local_getNameList;
//static ByteBuffer* viewresp = NULL;


/*******************************************************************************
	Globally viewed routines definitions
*******************************************************************************/

//CHANGE _V_0_3_0_
void
	mmsServer_handleGetNameListRequest( MmsServerConnection* connection, 
		GetNameListRequest_t* getNameList, int invokeId, ByteBuffer* response) {
	
	long objectClass;

	asn_INTEGER2long(&getNameList->objectClass.choice.basicObjectClass, &objectClass);

	long objectScope;

	switch (getNameList->objectScope.present) {
	case objectScope_PR_NOTHING:
		objectScope = 0;
		break;
	case objectScope_PR_vmdSpecific:
		objectScope = 1;
		break;
	case objectScope_PR_domainSpecific:
		objectScope = 2;
		break;
	case objectScope_PR_aaSpecific:
		objectScope = 3;
		break;
	default:
		objectScope = -1;
		break;
	}

	char* continueAfter = NULL;

	//TODO handle continueAfter
	if (getNameList->continueAfter != NULL) {
		int identifierLength = getNameList->continueAfter->size;
		continueAfter = malloc(identifierLength + 1);

		memcpy(continueAfter, getNameList->continueAfter->buf, identifierLength);
		continueAfter[identifierLength] = 0;
	}

	char* domainSpecificName = NULL;

	if (objectScope == 2) {

		domainSpecificName = createStringFromBuffer(
				getNameList->objectScope.choice.domainSpecific.buf,
				getNameList->objectScope.choice.domainSpecific.size);

		if (objectClass == basicObjectClass_namedVariable) {
			LinkedList nameList = getNameListDomainSpecific(connection, domainSpecificName);

			if (nameList == NULL)
				mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_TYPE_OBJECT_NON_EXISTENT);
			else {
				createNameListResponse(connection, invokeId, nameList, response, continueAfter);
				LinkedList_destroy(nameList);
			}
		}
#if (MMS_DATA_SET_SERVICE==1)
		else if (objectClass == basicObjectClass_namedVariableList) {
			LinkedList nameList = getNamedVariableListDomainSpecific(connection, domainSpecificName);

			createNameListResponse(connection, invokeId, nameList, response, continueAfter);

			LinkedList_destroy(nameList);
		}
#endif
		else {
			//if (DEBUG) printf("mms_server: getNameList objectClass %li not supported!\n", objectClass);

			mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_TYPE_OBJECT_ACCESS_UNSUPPORTED);
		}

		free(domainSpecificName);
	}

	else if (objectScope == 1) {

		LinkedList nameList = getNameListVMDSpecific(connection);

		createNameListResponse(connection, invokeId, nameList, response, continueAfter);

		LinkedList_destroyStatic(nameList);
	}
#if (MMS_DATA_SET_SERVICE==1)
	else if (objectScope == 3) {

		if (objectClass == basicObjectClass_namedVariableList) {
			LinkedList nameList = getNamedVariableListAssociationSpecific(connection);

			createNameListResponse(connection, invokeId, nameList, response, continueAfter);

			//LinkedList_destroyStatic(nameList);
			LinkedList_destroy(nameList);
		}
		else
			mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_TYPE_OBJECT_ACCESS_UNSUPPORTED);
	}
#endif
	else {
		//if (DEBUG) printf("mms_server: getNameList(%li) not supported!\n", objectScope);
		mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_TYPE_OBJECT_ACCESS_UNSUPPORTED);
	}

	if (continueAfter != NULL)
		free(continueAfter);
}


/*******************************************************************************
	Locally viewed (static) routines definitions
*******************************************************************************/

//CHANGE _V_0_3_0_
static LinkedList
getNameListDomainSpecific(MmsServerConnection* connection, char* domainName) {
	MmsDevice* device = MmsServer_getDevice(connection->server);

	LinkedList nameList = NULL;

	MmsDomain* domain = MmsDevice_getDomain(device, domainName);

	if (domain != NULL) {
		nameList = LinkedList_create();
		MmsTypeSpecification** variables = domain->namedVariables;

		int i;

		LinkedList element = nameList;

		for (i = 0; i < domain->namedVariablesCount; i++) {
			// LinkedList_add(nameList, copyString(variables[i]->name));
			element = LinkedList_insertAfter(element, copyString(variables[i]->name));

			char* prefix = variables[i]->name;

			//addSubNamedVaribleNamesToList(nameList, prefix, variables[i]);
			element = addSubNamedVaribleNamesToList(element, prefix, variables[i]);
		}
	}

	return nameList;
}


//CHANGE _VER_0_4_0_
static void
	createNameListResponse(MmsServerConnection* connection, int invokeId,
		LinkedList nameList, ByteBuffer* response, char* continueAfter) {
    
		LinkedList startElement = NULL;

    if (continueAfter != NULL) {
        LinkedList element = nameList;

        while ((element = LinkedList_getNext(element)) != NULL) {
            if (strcmp((char*) (element->data), continueAfter) == 0) {
                startElement = element;
                break;
            }
        }

        if (startElement == NULL) {
            mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_TYPE_OBJECT_ACCESS_UNSUPPORTED);
            return;
        }
    }

    /* determine number of identifiers to include in response */
    if (startElement == NULL)
        startElement = nameList;

    int nameCount = 0;
    int mmsPduLength = 25; /* estimated overhead size of PDU encoding */
    int maxPduSize = connection->maxPduSize;

    bool moreFollows = false;

    LinkedList element = startElement;

    while ((element = LinkedList_getNext(element)) != NULL) {
        int elementLength;

        elementLength = BerEncoder_determineEncodedStringSize((char*) element->data);

        if ((mmsPduLength + elementLength) > maxPduSize) {
            moreFollows = true;
            break;
        }
        else {
            mmsPduLength += elementLength;
            nameCount++;
        }

    }

    uint32_t identifierListSize = mmsPduLength - 25;

    uint32_t listOfIdentifierSize = 1 + BerEncoder_determineLengthSize(identifierListSize) + identifierListSize;

    uint32_t getNameListSize = listOfIdentifierSize + 3;

    uint32_t confirmedServiceResponseSize = 1 + BerEncoder_determineLengthSize(getNameListSize) + getNameListSize;

    uint32_t invokeIdSize = BerEncoder_UInt32determineEncodedSize((uint32_t) invokeId) + 2;

    uint32_t confirmedResponsePDUSize = confirmedServiceResponseSize + invokeIdSize;

    if (DEBUG) printf("maxPduLength: %i\n", maxPduSize);

    if (DEBUG) printf("mmsPduLength: %i (count = %i : more %i)\n",
            mmsPduLength, nameCount, moreFollows);

    /* encode response */
    element = startElement;

    uint8_t* buffer = response->buffer;
    int bufPos = 0;

    bufPos = BerEncoder_encodeTL(0xa1, confirmedResponsePDUSize, buffer, bufPos);

    bufPos = BerEncoder_encodeTL(0x02, invokeIdSize - 2, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32((uint32_t) invokeId, buffer, bufPos);

    bufPos = BerEncoder_encodeTL(0xa1, getNameListSize, buffer, bufPos);
    bufPos = BerEncoder_encodeTL(0xa0, identifierListSize, buffer, bufPos);

    int i = 0;

    while ((element = LinkedList_getNext(element)) != NULL) {
        bufPos = BerEncoder_encodeStringWithTag(0x1a, (char*) element->data, buffer, bufPos);

        i++;

        if (i == nameCount)
            break;
    }

    bufPos = BerEncoder_encodeBoolean(0x81, moreFollows, buffer, bufPos);

    response->size = bufPos;
}


static LinkedList
	getNameListVMDSpecific(MmsServerConnection* connection) {
	
	MmsDevice* device = MmsServer_getDevice(connection->server);

	LinkedList list = LinkedList_create();

	int i;

	for (i = 0; i < device->domainCount; i++) {
		LinkedList_add(list, device->domains[i]->domainName);
	}

	return list;
}


//CHANGE _V_0_3_0_
static LinkedList
addSubNamedVaribleNamesToList(LinkedList nameList, char* prefix, 
	MmsTypeSpecification* variable)
{
	LinkedList listElement = nameList;

	if (variable->type == MMS_STRUCTURE) {

		int i;

		MmsTypeSpecification** variables = variable->typeSpec.structure.elements;

		for (i = 0; i < variable->typeSpec.structure.elementCount; i++) {
			//char* variableName = createString(3, prefix, "$", variables[i]->name);
			char* variableName = appendMmsSubVariable(prefix, variables[i]->name);
			//printf("item: %s\n", variableName);

			//LinkedList_add(nameList, variableName);
			listElement = LinkedList_insertAfter(listElement, variableName);

			listElement = addSubNamedVaribleNamesToList(listElement, variableName, variables[i]);
		}
	}
	else if (variable->type == MMS_ARRAY) {
		MmsTypeSpecification* element = variable->typeSpec.array.elementTypeSpec;
		listElement = addSubNamedVaribleNamesToList(listElement, prefix, element);
	}

	return listElement;
}


#if (MMS_DATA_SET_SERVICE==1)

static LinkedList
	createStringsFromNamedVariableList(LinkedList nameList, LinkedList 
		variableLists) {
	nameList = LinkedList_create();
	LinkedList variableListsElement = LinkedList_getNext(variableLists);
	while (variableListsElement != NULL ) {
		MmsNamedVariableList variableList =
				(MmsNamedVariableList) variableListsElement->data;

		LinkedList_add(nameList,
				copyString(MmsNamedVariableList_getName(variableList)));

		variableListsElement = LinkedList_getNext(variableListsElement);
	}
	return nameList;
}

static LinkedList
	getNamedVariableListDomainSpecific(MmsServerConnection* connection, char* 
		domainName) {
	
	MmsDevice* device = MmsServer_getDevice(connection->server);

	LinkedList nameList = NULL;

	MmsDomain* domain = MmsDevice_getDomain(device, domainName);

	if (domain != NULL) {
		LinkedList variableLists = MmsDomain_getNamedVariableLists(domain);

		nameList = createStringsFromNamedVariableList(nameList, variableLists);
	}

	return nameList;
}

static LinkedList
	getNamedVariableListAssociationSpecific(MmsServerConnection* connection) {
	LinkedList nameList = NULL;

	LinkedList variableLists = MmsServerConnection_getNamedVariableLists(connection);

	nameList = createStringsFromNamedVariableList(nameList, variableLists);

	return nameList;
}
#endif

static char* appendMmsSubVariable(char* name, char* child)
{
    int nameLen = strlen(name);
    int childLen = strlen(child);

    int newSize = nameLen + childLen + 2;

    char* newName = malloc(newSize);

    int bufPos = 0;
    int i;
    for (i = 0; i < nameLen; i++) {
        newName[bufPos++] = name[i];
    }
    newName[bufPos++] = '$';

    for (i = 0; i < childLen; i++) {
        newName[bufPos++] = child[i];
    }

    newName[bufPos] = 0;

    return newName;
}

