#include "libiec61850/mms/mms_server_common.h"
#include "iso9506/ServiceError.h"
#include "libiec61850/utils/linked_list.h"
#include "libiec61850/mms/mms_common.h"
#include "libiec61850/mms/mms_named_variable_list.h"


//CHANGE _V_0_3_0_
void
mmsServer_createConfirmedErrorPdu(int invokeId, ByteBuffer* response,
	MmsConfirmedErrorType errorType){

	MmsPdu_t* mmsPdu = calloc(1, sizeof(MmsPdu_t));
	mmsPdu->present = MmsPdu_PR_confirmedErrorPDU;

	asn_long2INTEGER(&(mmsPdu->choice.confirmedErrorPDU.invokeID),
			invokeId);

	if (errorType == MMS_ERROR_TYPE_OBJECT_NON_EXISTENT) {
		mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.present =
				errorClass_PR_access;

	//mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.choice.access =
	//		access_objectnonexistent;
	asn_long2INTEGER(&mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.choice.access,
		access_objectnonexistent);
	}
	else if (errorType == MMS_ERROR_TYPE_ACCESS_DENIED) {
		mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.present =
				errorClass_PR_access;

		//mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.choice.access =
		//	access_objectaccessdenied;
		asn_long2INTEGER(&mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.choice.access,
			access_objectaccessdenied);
	}
	else if (errorType == MMS_ERROR_TYPE_OBJECT_ACCESS_UNSUPPORTED) {
		mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.present =
				errorClass_PR_access;
		//mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.choice.access =
		//	access_objectaccessunsupported;
		asn_long2INTEGER(&mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.choice.access,
			access_objectaccessunsupported);
	} 
	else if (errorType == MMS_ERROR_TYPE_RESPONSE_EXCEEDS_MAX_PDU_SIZE) {
		mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.present =
			errorClass_PR_service;
						//ServiceError__errorClass_PR_service;
		asn_long2INTEGER(&mmsPdu->choice.confirmedErrorPDU.serviceError.errorClass.choice.access,
						service_other);
	}

	der_encode(&asn_DEF_MmsPdu, mmsPdu, mmsServer_write_out, (void*) response);

	asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);
}


int
mmsServer_write_out(const void *buffer, size_t size, void *app_key)
{
    ByteBuffer* writeBuffer = (ByteBuffer*) app_key;
    return ByteBuffer_append(writeBuffer, (uint8_t*)buffer, size);
}


MmsPdu_t*
mmsServer_createConfirmedResponse(int invokeId)
{
	MmsPdu_t* mmsPdu = calloc(1, sizeof(MmsPdu_t));

	mmsPdu->present = MmsPdu_PR_confirmedResponsePdu;

	asn_long2INTEGER(&(mmsPdu->choice.confirmedResponsePdu.invokeID),
		invokeId);

	return mmsPdu;
}


//CHANGE _V_0_3_0_
int
mmsServer_isIndexAccess(AlternateAccess_t* alternateAccess)
{
	if (alternateAccess->list.array[0]->present == AlternateAccessChoice_PR_unnamed) {
		if ((alternateAccess->list.array[0]->choice.unnamed.choice.selectAccess.present
				== selectAccess_PR_index) ||
			(alternateAccess->list.array[0]->choice.unnamed.choice.selectAccess.present
				== selectAccess_PR_indexRange))
		{
			return 1;
		}
		else
			return 0;
	}
	else
		return 0;
}

//CHANGE _V_0_3_0_
int
mmsServer_getLowIndex(AlternateAccess_t* alternateAccess)
{
	if (alternateAccess->list.array[0]->choice.unnamed.choice.selectAccess.present
				== selectAccess_PR_index)
	{
		long index;
		asn_INTEGER2long(
				&alternateAccess->list.array[0]->choice.unnamed.choice.selectAccess.choice.index,
				&index);

		return (int) index;
	}

	if (alternateAccess->list.array[0]->choice.unnamed.choice.selectAccess.present
				== selectAccess_PR_indexRange)
	{
		long index;
		asn_INTEGER2long(
				&alternateAccess->list.array[0]->choice.unnamed.choice.selectAccess.choice.indexRange.lowIndex,
				&index);

		return (int) index;
	}

	return -1;
}


//CHANGE _V_0_3_0_
int
mmsServer_getNumberOfElements(AlternateAccess_t* alternateAccess)
{
	if (alternateAccess->list.array[0]->choice.unnamed.choice.selectAccess.present
				== selectAccess_PR_indexRange)
	{
		long number;

		asn_INTEGER2long(
				&alternateAccess->list.array[0]->choice.unnamed.choice.selectAccess.choice.indexRange.numberOfElements,
				&number);

		return (int) number;
	}

	return 0;
}


void
mmsServer_deleteVariableList(LinkedList namedVariableLists, char* variableListName)
{
	LinkedList previousElement = namedVariableLists;
	LinkedList element = LinkedList_getNext(namedVariableLists);

	while (element != NULL ) {
		MmsNamedVariableList varList = (MmsNamedVariableList) element->data;

		if (strcmp(MmsNamedVariableList_getName(varList), variableListName)
				== 0) {
			previousElement->next = element->next;
			free(element);
			MmsNamedVariableList_destroy(varList);

			break;
		}

		previousElement = element;
		element = LinkedList_getNext(element);
	}
}

