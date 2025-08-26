#ifndef __MMS_SERVER_COMMON_H__
#define __MMS_SERVER_COMMON_H__


/*******************************************************************************
	Defines
*******************************************************************************/


#include "iso9506/MmsPdu.h"
#include "libiec61850/utils/byte_buffer.h" //#include "server/byte_buffer.h"
//#include "server/mms_server_internal.h" // закоменчен ранее
#include "libiec61850/utils/linked_list.h"


/*******************************************************************************
	Types declarations
*******************************************************************************/


typedef enum {
	MMS_ERROR_TYPE_OK,
	MMS_ERROR_TYPE_OBJECT_NON_EXISTENT,
	MMS_ERROR_TYPE_OBJECT_ACCESS_UNSUPPORTED,
	MMS_ERROR_TYPE_ACCESS_DENIED,
	MMS_ERROR_TYPE_RESPONSE_EXCEEDS_MAX_PDU_SIZE
} MmsConfirmedErrorType;


/*******************************************************************************
	Globally viewed routines prototypes
*******************************************************************************/


void mmsServer_createConfirmedErrorPdu(int invokeId, ByteBuffer* response, MmsConfirmedErrorType errorType);

int	mmsServer_write_out(const void *buffer, size_t size, void *app_key);
	
MmsPdu_t*
	mmsServer_createConfirmedResponse(int invokeId);

int
	mmsServer_isIndexAccess(AlternateAccess_t* alternateAccess);
	
int
	mmsServer_getLowIndex(AlternateAccess_t* alternateAccess);
	
int
	mmsServer_getNumberOfElements(AlternateAccess_t* alternateAccess);
	
void mmsServer_deleteVariableList(LinkedList, char*);

#endif /*__MMS_SERVER_COMMON_H__*/