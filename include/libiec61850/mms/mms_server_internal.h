#ifndef __MMS_SERVER_INTERNAL_H__
#define __MMS_SERVER_INTERNAL_H__

//#include "mms_server_connection.h"
#include "libiec61850/utils/linked_list.h"

/*typedef enum {
	MMS_ERROR_TYPE_OBJECT_NON_EXISTENT,
	MMS_ERROR_TYPE_OBJECT_ACCESS_UNSUPPORTED,
	MMS_ERROR_TYPE_ACCESS_DENIED
} MmsConfirmedErrorType;*/

void
mmsServer_deleteVariableList(LinkedList namedVariableLists, char* variableListName);

#endif /* __MMS_SERVER_INTERNAL_H__ */
