/**
	******************************************************************************
  * @file    mms_service_named_variable_list.c
  * @author  Michael Zillgith
  * @brief   This file is part of libIEC61850.
  *****************************************************************************/

#include "libiec61850/mms/mms_service_named_variable_list.h"
#include "stack_config.h"
//#include <iso9506/MmsPdu.h>
#include "libiec61850/utils/string_utilities.h"
//#include "ied/mms/mms_named_variable_list.h"
//#include "ied/mms/mms_server_common.h"
//#include "ied/utility/ber_encoder.h"
#include "libiec61850/mms/mms_device.h"
#include "libiec61850/mms/mms_server.h"
#include "libiec61850/mms/mms_server_common.h"

/*******************************************************************************
	Locally viewed (static) routines prototypes
*******************************************************************************/

//static int
//	createVariableAccessAttributesResponse( MmsServerConnection* connection, char* 
//		domainId,	char* nameId,	int invokeId,	ByteBuffer* response);
		
//static int
//	createTypeSpecification( MmsTypeSpecification* namedVariable, 
//		TypeSpecification_t* typeSpec);
		
//static void
//	deleteVariableAccessAttributesResponse( GetVariableAccessAttributesResponse_t*
//		getVarAccessAttr);
		
//static void
//	freeTypeSpecRecursive(TypeSpecification_t* typeSpec);
		
static void
	createGetNamedVariableListAttributesResponse(int invokeId, ByteBuffer* 
		response, MmsNamedVariableList variableList, char* domainName) ;


//static void
//	createDefineNamedVariableListResponse(int invokeId, ByteBuffer* response);
	
//static void
//	createDeleteNamedVariableListResponse(uint32_t invokeId, ByteBuffer* response,
//		uint32_t numberMatched, uint32_t numberDeleted);
		
//static MmsNamedVariableList
//	createNamedVariableList(MmsDevice* device, DefineNamedVariableListRequest_t* 
//		request, char* variableListName);
	
	
/*******************************************************************************
	Globally viewed routines definitions
*******************************************************************************/

#if (MMS_DATA_SET_SERVICE == 1)
//#error "MMS_DATA_SET_SERVICE"
/*	
void
	mmsServer_handleDeleteNamedVariableListRequest(MmsServerConnection* connection,
		DeleteNamedVariableListRequest_t* request, int invokeId, ByteBuffer* 
		response) {
	//TODO implement for association specific named variable lists

	//CHANGE _V_0_3_1_
	//request->scopeOfDelete;

	long scopeOfDelete;

	asn_INTEGER2long(request->scopeOfDelete, &scopeOfDelete);

	MmsDevice* device = MmsServer_getDevice(connection->server);

	if (scopeOfDelete == DeleteNamedVariableListRequest__scopeOfDelete_specific) {
		int numberMatched = 0;
		int numberDeleted = 0;

		int numberItems = request->listOfVariableListName->list.count;

		int i;

		for (i = 0; i < numberItems; i++) {
			if (request->listOfVariableListName->list.array[i]->present == ObjectName_PR_domainspecific) {
				char* domainId = mmsMsg_createStringFromAsnIdentifier(
						request->listOfVariableListName->list.array[i]->choice.domainspecific.domainId);

				MmsDomain* domain = MmsDevice_getDomain(device, domainId);

				char* itemId = mmsMsg_createStringFromAsnIdentifier(
						request->listOfVariableListName->list.array[i]->choice.domainspecific.itemId);

				MmsNamedVariableList variableList = MmsDomain_getNamedVariableList(domain, itemId);

				if (variableList != NULL) {
					numberMatched++;

					if (MmsNamedVariableList_isDeletable(variableList)) {
						MmsDomain_deleteNamedVariableList(domain, itemId);
						numberDeleted++;
					}
				}

				free(domainId);
				free(itemId);
			}
			else if (request->listOfVariableListName->list.array[i]->present == ObjectName_PR_aaspecific) {
				char* itemId = mmsMsg_createStringFromAsnIdentifier(
						request->listOfVariableListName->list.array[i]->choice.aaspecific);

				MmsNamedVariableList variableList = MmsServerConnection_getNamedVariableList(connection, itemId);

				if (variableList != NULL) {
					numberMatched++;
					numberDeleted++;

					MmsServerConnection_deleteNamedVariableList(connection, itemId);
				}

				free(itemId);
			}
			//TODO else send error???
		}

		createDeleteNamedVariableListResponse(invokeId, response, numberMatched, numberDeleted);
	}
	else {
		mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_TYPE_OBJECT_ACCESS_UNSUPPORTED);
	}
}*/


void
	mmsServer_handleGetNamedVariableListAttributesRequest(MmsServerConnection* 
		connection, GetNamedVariableListAttributesRequest_t* request, int invokeId,
		ByteBuffer* response) {
	
	if (request->present == ObjectName_PR_domainspecific) {

		char* domainName = createStringFromBuffer(
				request->choice.domainspecific.domainId.buf,
				request->choice.domainspecific.domainId.size);

		char* itemName = createStringFromBuffer(
				request->choice.domainspecific.itemId.buf,
				request->choice.domainspecific.itemId.size);

		MmsDevice* mmsDevice = MmsServer_getDevice(connection->server);

		MmsDomain* domain = MmsDevice_getDomain(mmsDevice, domainName);

		if (domain != NULL) {
			MmsNamedVariableList variableList =
					MmsDomain_getNamedVariableList(domain, itemName);

			if (variableList != NULL)
				createGetNamedVariableListAttributesResponse(invokeId, response, variableList, domainName);
			else
				mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_TYPE_OBJECT_NON_EXISTENT);
		}
		else
			mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_TYPE_OBJECT_NON_EXISTENT);


		free(domainName);
		free(itemName);
	}
	else {
		mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_TYPE_OBJECT_ACCESS_UNSUPPORTED);
	}
}

/*
void
	mmsServer_handleDefineNamedVariableListRequest(MmsServerConnection* 
		connection, DefineNamedVariableListRequest_t* request,	int invokeId,
		ByteBuffer* response) {
			
	MmsDevice* device = MmsServer_getDevice(connection->server);

	if (request->variableListName.present == ObjectName_PR_domainspecific) {

		char* domainName = createStringFromBuffer(
				request->variableListName.choice.domainspecific.domainId.buf,
				request->variableListName.choice.domainspecific.domainId.size);

		MmsDomain* domain = MmsDevice_getDomain(device, domainName);

		free(domainName);

		if (domain == NULL) {
			mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_TYPE_OBJECT_NON_EXISTENT);
			return;
		}

		char* variableListName = createStringFromBuffer(
				request->variableListName.choice.domainspecific.itemId.buf,
				request->variableListName.choice.domainspecific.itemId.size);

		MmsNamedVariableList namedVariableList = createNamedVariableList(device,
				request, variableListName);

		free(variableListName);

		if (namedVariableList != NULL) {
			MmsDomain_addNamedVariableList(domain, namedVariableList);
			createDefineNamedVariableListResponse(invokeId, response);
		}
		else
			mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_TYPE_OBJECT_ACCESS_UNSUPPORTED);

	}
	else if (request->variableListName.present == ObjectName_PR_aaspecific) {

		char* variableListName = createStringFromBuffer(
				request->variableListName.choice.aaspecific.buf,
				request->variableListName.choice.aaspecific.size);

		MmsNamedVariableList namedVariableList = createNamedVariableList(device,
				request, variableListName);

		if (namedVariableList != NULL) {
			MmsServerConnection_addNamedVariableList(connection, namedVariableList);
			createDefineNamedVariableListResponse(invokeId, response);
		}
		else
			mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_TYPE_OBJECT_ACCESS_UNSUPPORTED);

		free(variableListName);
	}
	else
		mmsServer_createConfirmedErrorPdu(invokeId, response, MMS_ERROR_TYPE_OBJECT_ACCESS_UNSUPPORTED);
}


int
	mmsServer_handleGetVariableAccessAttributesRequest(MmsServerConnection* 
		connection,	GetVariableAccessAttributesRequest_t* request, int invokeId,
		ByteBuffer* response) {
	
	if (request->present == GetVariableAccessAttributesRequest_PR_name) {
		if (request->choice.name.present == ObjectName_PR_domainspecific) {
			Identifier_t domainId = request->choice.name.choice.domainspecific.domainId;
			Identifier_t nameId = request->choice.name.choice.domainspecific.itemId;

			char* domainIdStr = createStringFromBuffer(domainId.buf, domainId.size);
			char* nameIdStr = createStringFromBuffer(nameId.buf, nameId.size);
			if (ISO9506_DEBUG) printf("getVariableAccessAttributes domainId: %s nameId: %s\r\n", domainIdStr, nameIdStr);

			createVariableAccessAttributesResponse(connection, domainIdStr, nameIdStr, invokeId, response);

			free(domainIdStr);
			free(nameIdStr);
		}
		else {
			if (ISO9506_DEBUG) printf("GetVariableAccessAttributesRequest with name other than domainspecific is not supported!\r\n");
			return -1;
		}
	}
	else {
		if (ISO9506_DEBUG) printf("GetVariableAccessAttributesRequest with address not supported!\r\n");
		return -1;
	}

	return 0;
}
*/
#endif /*(MMS_DATA_SET_SERVICE == 1)*/


/*******************************************************************************
	Locally viewed (static) routines definitions
*******************************************************************************/

#if (MMS_DATA_SET_SERVICE == 1)

//CHANGE _V_0_3_1_
/*
static void
createDeleteNamedVariableListResponse(uint32_t invokeId, ByteBuffer* response,
        uint32_t numberMatched, uint32_t numberDeleted)
{
    uint32_t invokeIdSize = BerEncoder_UInt32determineEncodedSize(invokeId) + 2;

    uint32_t numberMatchedSize =
            2 + BerEncoder_UInt32determineEncodedSize(numberMatched);

    uint32_t numberDeletedSize =
            2 + BerEncoder_UInt32determineEncodedSize(numberDeleted);

    uint32_t deleteNVLSize = 2 + numberMatchedSize + numberDeletedSize;

    uint32_t confirmedResponsePDUSize = invokeIdSize + deleteNVLSize;

    int bufPos = 0;
    uint8_t* buffer = response->buffer;

    bufPos = BerEncoder_encodeTL(0xa1, confirmedResponsePDUSize, buffer, bufPos);

    bufPos = BerEncoder_encodeTL(0x02, invokeIdSize - 2, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(invokeId, buffer, bufPos);

    bufPos = BerEncoder_encodeTL(0xad, numberMatchedSize + numberDeletedSize, buffer, bufPos);

    bufPos = BerEncoder_encodeTL(0x80, numberMatchedSize - 2, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(numberMatched, buffer, bufPos);

    bufPos = BerEncoder_encodeTL(0x81, numberDeletedSize - 2, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32(numberDeleted, buffer, bufPos);

    response->size = bufPos;
}*/


//CHANGE _V_0_3_1_
/*
static void
createDefineNamedVariableListResponse(int invokeId, ByteBuffer* response)
{
    uint32_t invokeIdSize = BerEncoder_UInt32determineEncodedSize((uint32_t) invokeId) + 2;

    uint32_t confirmedResponsePDUSize = 2 + invokeIdSize;

    int bufPos = 0;
    uint8_t* buffer = response->buffer;

    bufPos = BerEncoder_encodeTL(0xa1, confirmedResponsePDUSize, buffer, bufPos);

    bufPos = BerEncoder_encodeTL(0x02, invokeIdSize - 2, buffer, bufPos);
    bufPos = BerEncoder_encodeUInt32((uint32_t) invokeId, buffer, bufPos);

    bufPos = BerEncoder_encodeTL(0x8b, 0, buffer, bufPos);

    response->size = bufPos;
}*/

//updated v_0_3_3
static void
	createGetNamedVariableListAttributesResponse(int invokeId, ByteBuffer* 
		response, MmsNamedVariableList variableList, char* domainName) {
	
	MmsPdu_t* mmsPdu = mmsServer_createConfirmedResponse(invokeId);

	BOOLEAN_t moreFollows = 0;

	mmsPdu->choice.confirmedResponsePdu.confirmedServiceResponse.present =
			ConfirmedServiceResponse_PR_getNamedVariableListAttributes;

	GetNamedVariableListAttributesResponse_t* varListResponse =
		&(mmsPdu->choice.confirmedResponsePdu.confirmedServiceResponse.
				choice.getNamedVariableListAttributes);

	varListResponse->mmsDeletable = MmsNamedVariableList_isDeletable(variableList);

	LinkedList variables = MmsNamedVariableList_getVariableList(variableList);

	int variableCount = LinkedList_size(variables);

	varListResponse->listOfVariable.list.count = variableCount;
	varListResponse->listOfVariable.list.size = variableCount;

	varListResponse->listOfVariable.list.array = calloc(variableCount, sizeof(void*));

	LinkedList variable = LinkedList_getNext(variables);

	int i;
	for (i = 0; i < variableCount; i++) {
		MmsNamedVariableListEntry variableEntry = (MmsNamedVariableListEntry) variable->data;

		varListResponse->listOfVariable.list.array[i] =  calloc(1,
				sizeof(struct ListOfVariableSeq));

		varListResponse->listOfVariable.list.array[i]->variableSpecification.present =
				VariableSpecification_PR_name;

		varListResponse->listOfVariable.list.array[i]->variableSpecification.choice.name.present =
				ObjectName_PR_domainspecific;

		char* variableDomainName = MmsDomain_getName(variableEntry->domain);

		varListResponse->listOfVariable.list.array[i]->variableSpecification.choice.name.choice.
			domainspecific.domainId.buf = (uint8_t*)copyString(variableDomainName);

		varListResponse->listOfVariable.list.array[i]->variableSpecification.choice.name.choice.
			domainspecific.domainId.size = strlen(variableDomainName);

		varListResponse->listOfVariable.list.array[i]->variableSpecification.choice.name.choice.
			domainspecific.itemId.buf = (uint8_t*)copyString(variableEntry->variableName);

		varListResponse->listOfVariable.list.array[i]->variableSpecification.choice.name.choice.
			domainspecific.itemId.size = strlen(variableEntry->variableName);

		variable = LinkedList_getNext(variable);
	}

	asn_enc_rval_t rval;

	rval = der_encode(&asn_DEF_MmsPdu, mmsPdu,
			mmsServer_write_out, (void*) response);

//	if (DEBUG) xer_fprint(stdout, &asn_DEF_MmsPdu, mmsPdu);


	asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);
}

/*
static int
	createVariableAccessAttributesResponse(MmsServerConnection* connection,
		char* domainId,	char* nameId,	int invokeId,	ByteBuffer* response) {
			
	MmsDevice* device = MmsServer_getDevice(connection->server);

	MmsDomain* domain = MmsDevice_getDomain(device, domainId);

	if (domain == NULL) {
//		if (DEBUG) printf("mms_server: domain %s not known\r\n", domainId);
		return -1;
	}

	MmsTypeSpecification* namedVariable = MmsDomain_getNamedVariable(domain, nameId);

	if (namedVariable == NULL) {
//		if (DEBUG) printf("mms_server: named variable %s not known\r\n", nameId);
		return -1;
	}

	MmsPdu_t* mmsPdu = mmsServer_createConfirmedResponse(invokeId);

	mmsPdu->choice.confirmedResponsePdu.confirmedServiceResponse.present =
			ConfirmedServiceResponse_PR_getVariableAccessAttributes;

	GetVariableAccessAttributesResponse_t* getVarAccessAttr;

	getVarAccessAttr = &(mmsPdu->choice.confirmedResponsePdu.
			confirmedServiceResponse.choice.getVariableAccessAttributes);

	getVarAccessAttr->mmsDeletable = 0;

	createTypeSpecification(namedVariable, &getVarAccessAttr->typeSpecification);

	asn_enc_rval_t rval;

	rval = der_encode(&asn_DEF_MmsPdu, mmsPdu,
			mmsServer_write_out, (void*) response);

//	if (DEBUG) xer_fprint(stdout, &asn_DEF_MmsPdu, mmsPdu);

	deleteVariableAccessAttributesResponse(getVarAccessAttr);

	asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);

	return 0;
}*/

/*
static int
	createTypeSpecification (MmsTypeSpecification* namedVariable, 
		TypeSpecification_t* typeSpec) {

	if (namedVariable->type == MMS_ARRAY) {
		typeSpec->present = TypeSpecification_PR_array;
		//typeSpec->choice.array.numberOfElements;

		asn_long2INTEGER(&(typeSpec->choice.array.numberOfElements),
				(long) namedVariable->typeSpec.array.elementCount);

		typeSpec->choice.array.packed = NULL;
		typeSpec->choice.array.elementType = calloc(1, sizeof(TypeSpecification_t));

		createTypeSpecification(namedVariable->typeSpec.array.elementTypeSpec,
				typeSpec->choice.array.elementType);
	}
	else if (namedVariable->type == MMS_STRUCTURE) {

		typeSpec->present = TypeSpecification_PR_structure;

		int componentCount = namedVariable->typeSpec.structure.elementCount;

		typeSpec->choice.structure.components.list.count = componentCount;
		typeSpec->choice.structure.components.list.size = componentCount;

		typeSpec->choice.structure.components.list.array
			= calloc(componentCount, sizeof(StructComponent_t*));

		int i;

		for (i = 0; i < componentCount; i++) {

			typeSpec->choice.structure.components.list.array[i] =
					calloc(1, sizeof(StructComponent_t));

			typeSpec->choice.structure.components.list.array[i]->componentName =
					calloc(1, sizeof(Identifier_t));

			typeSpec->choice.structure.components.list.array[i]->componentName->buf =
					(uint8_t*)namedVariable->typeSpec.structure.elements[i]->name;

			typeSpec->choice.structure.components.list.array[i]->componentName->size =
					strlen(namedVariable->typeSpec.structure.elements[i]->name);

			typeSpec->choice.structure.components.list.array[i]->componentType =
					calloc(1, sizeof(TypeSpecification_t));

			createTypeSpecification(namedVariable->typeSpec.structure.elements[i],
					typeSpec->choice.structure.components.list.array[i]->componentType);
		}
	}
	else {

//		typedef enum TypeSpecification_PR {
//			TypeSpecification_PR_NOTHING,	// No components present
//			TypeSpecification_PR_typeName,
//			TypeSpecification_PR_array,
//			TypeSpecification_PR_structure,
//			TypeSpecification_PR_boolean,
//			TypeSpecification_PR_bitstring,
//			TypeSpecification_PR_integer,
//			TypeSpecification_PR_unsigned,
//			TypeSpecification_PR_floatingpoint,
//			TypeSpecification_PR_octetstring,
//			TypeSpecification_PR_visiblestring,
//			TypeSpecification_PR_generalizedtime,
//			TypeSpecification_PR_binarytime,
//			TypeSpecification_PR_bcd,
//			TypeSpecification_PR_objId,
//			TypeSpecification_PR_mMSString,
//			TypeSpecification_PR_utctime
//		} TypeSpecification_PR;

		switch (namedVariable->type) {
		case MMS_BOOLEAN:
			typeSpec->present = TypeSpecification_PR_boolean;
			break;
		case MMS_BIT_STRING:
			typeSpec->present = TypeSpecification_PR_bitstring;
			typeSpec->choice.bitstring = namedVariable->typeSpec.bitString;
			break;
		case MMS_INTEGER:
			typeSpec->present = TypeSpecification_PR_integer;
			typeSpec->choice.integer = namedVariable->typeSpec.integer;
			break;
		case MMS_UNSIGNED:
			typeSpec->present = TypeSpecification_PR_unsigned;
			typeSpec->choice.Unsigned = namedVariable->typeSpec.unsignedInteger;
			break;
		case MMS_FLOAT:
			typeSpec->present = TypeSpecification_PR_floatingpoint;
			typeSpec->choice.floatingpoint.exponentwidth =
					namedVariable->typeSpec.floatingpoint.exponentWidth;
			typeSpec->choice.floatingpoint.formatwidth =
					namedVariable->typeSpec.floatingpoint.formatWidth;
			break;
		case MMS_OCTET_STRING:
			typeSpec->present = TypeSpecification_PR_octetstring;
			typeSpec->choice.octetstring = namedVariable->typeSpec.octetString;
			break;
		case MMS_VISIBLE_STRING:
			typeSpec->present = TypeSpecification_PR_visiblestring;
			typeSpec->choice.visiblestring = namedVariable->typeSpec.visibleString;
			break;
		case MMS_STRING:
			typeSpec->present = TypeSpecification_PR_mMSString;
			typeSpec->choice.mMSString = namedVariable->typeSpec.mmsString;
			break;
		case MMS_UTC_TIME:
			typeSpec->present = TypeSpecification_PR_utctime;
			break;
		default:
			if (ISO9506_DEBUG) printf("MMS-SERVER: Unsupported type %i!\r\n", namedVariable->type);
			return -1;
			break;
		}
	}

	return 1;
}*/

/*
static void
	deleteVariableAccessAttributesResponse(GetVariableAccessAttributesResponse_t* 
		getVarAccessAttr) {
			
	if (getVarAccessAttr->typeSpecification.present	== TypeSpecification_PR_structure) {
		int size =	getVarAccessAttr->typeSpecification.choice.structure.components.list.size;
		int count =	getVarAccessAttr->typeSpecification.choice.structure.components.list.count;

		int i;
		for (i = 0; i < count; i++) {
			free(getVarAccessAttr->typeSpecification.choice.structure.components.list.array[i]->componentName);
			TypeSpecification_t* typeSpec =
					getVarAccessAttr->typeSpecification.choice.structure.components.list.array[i]->componentType;
			freeTypeSpecRecursive(typeSpec);
			free(typeSpec);
			free(getVarAccessAttr->typeSpecification.choice.structure.components.list.array[i]);
		}

		free(getVarAccessAttr->typeSpecification.choice.structure.components.list.array);

		getVarAccessAttr->typeSpecification.choice.structure.components.list.array = NULL;
		getVarAccessAttr->typeSpecification.choice.structure.components.list.count = 0;
		getVarAccessAttr->typeSpecification.choice.structure.components.list.size =	0;
	} else {
		if (ISO9506_DEBUG) printf("problem deleting ASN1 structure!\r\n");
	}
}*/

/*
static void
	freeTypeSpecRecursive(TypeSpecification_t* typeSpec) {
	
	if (typeSpec->present == TypeSpecification_PR_structure) {
		int elementCount =
				typeSpec->choice.structure.components.list.count;

		int i;

		for (i = 0; i < elementCount; i++) {
			free(typeSpec->choice.structure.components.list.array[i]->componentName);
			freeTypeSpecRecursive(typeSpec->choice.structure.components.list.array[i]->componentType);
			free(typeSpec->choice.structure.components.list.array[i]->componentType);
			free(typeSpec->choice.structure.components.list.array[i]);
		}

		free(typeSpec->choice.structure.components.list.array);
	}
	else if (typeSpec->present == TypeSpecification_PR_array) {
		free(typeSpec->choice.array.numberOfElements.buf);
		freeTypeSpecRecursive(typeSpec->choice.array.elementType);
		free(typeSpec->choice.array.elementType);
	}
}*/


//CHANGE _V_0_3_0_
/*
static MmsNamedVariableList
createNamedVariableList(MmsDevice* device,
		DefineNamedVariableListRequest_t* request,
		char* variableListName)
{
	MmsNamedVariableList namedVariableList = MmsNamedVariableList_create(variableListName, true);

	int variableCount = request->listOfVariable.list.count;

	int i;
	for (i = 0; i < variableCount; i++) {
		VariableSpecification_t* varSpec =
				&request->listOfVariable.list.array[i]->variableSpecification;

		long arrayIndex = -1;
		char* componentName = NULL;

		// Handle alternate access specification - for array element definitiion
		if (request->listOfVariable.list.array[i]->alternateAccess != NULL) {


			if (request->listOfVariable.list.array[i]->alternateAccess->list.count != 1) {
				MmsNamedVariableList_destroy(namedVariableList);
				namedVariableList = NULL;
				break;
			}
			else {

				struct AlternateAccess__Member* alternateAccess =
						request->listOfVariable.list.array[i]->alternateAccess->list.array[0];

				if (alternateAccess->present ==	AlternateAccess__Member_PR_unnamed)
				{
					//TODO add checks!

					asn_INTEGER2long(&(alternateAccess->choice.unnamed->choice.selectAlternateAccess.accessSelection.choice.index),
							&arrayIndex);

					printf("named variable element with array index %i\n", arrayIndex);

					componentName =
						mmsMsg_createStringFromAsnIdentifier(alternateAccess->choice.unnamed->
								choice.selectAlternateAccess.alternateAccess->list.array[0]->
								choice.unnamed->choice.selectAccess.choice.component);

				}
				else {
					MmsNamedVariableList_destroy(namedVariableList);
					namedVariableList = NULL;
					break;
				}

			}

		}

		if (varSpec->present == VariableSpecification_PR_name) {
			char* variableName = createStringFromBuffer(
					varSpec->choice.name.choice.domainspecific.itemId.buf,
					varSpec->choice.name.choice.domainspecific.itemId.size);

			char* domainId = createStringFromBuffer(
					varSpec->choice.name.choice.domainspecific.domainId.buf,
					varSpec->choice.name.choice.domainspecific.domainId.size);

			MmsDomain* domain = MmsDevice_getDomain(device, domainId);

			MmsAccessSpecifier accessSpecifier;

			accessSpecifier.domain = domain;
			accessSpecifier.variableName = variableName;
			accessSpecifier.arrayIndex = arrayIndex;
			accessSpecifier.componentName = componentName;

			MmsNamedVariableListEntry variable =
					MmsNamedVariableListEntry_create(accessSpecifier);

			MmsNamedVariableList_addVariable(namedVariableList, variable);

			free(domainId);
			free(variableName);
		}
		else {
			MmsNamedVariableList_destroy(namedVariableList);
			namedVariableList = NULL;
			break;
		}
	}

	return namedVariableList;
}*/

#endif /*MMS_DATA_SET_SERVICE*/
