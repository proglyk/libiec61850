#ifndef __MMS_SERVER_CONNECTION_H__
#define __MMS_SERVER_CONNECTION_H__

#include "ied/mms/mms_server.h"
#include "libiec61850/ied/mms/mms_common.h"
#include "libiec61850/utils/SBuffer.h"
/*#include "server/iso_server2.h"
#include "ied/mms/mms_types.h" */

/* #define MMS_SERVICE_STATUS 0x80
#define MMS_SERVICE_GET_NAME_LIST 0x40
#define MMS_SERVICE_IDENTIFY 0x20
#define MMS_SERVICE_RENAME 0x10
#define MMS_SERVICE_READ 0x08
#define MMS_SERVICE_WRITE 0x04
#define MMS_SERVICE_GET_VARIABLE_ACCESS_ATTRIBUTES 0x02
#define MMS_SERVICE_DEFINE_NAMED_VARIABLE 0x01

#define MMS_SERVICE_DEFINE_SCATTERED_ACCESS 0x80
#define MMS_SERVICE_GET_SCATTERED_ACCESS_ATTRIBUTES 0x40
#define MMS_SERVICE_DELETE_VARIABLE_ACCESS 0x20
#define MMS_SERVICE_DEFINE_NAMED_VARIABLE_LIST 0x10
#define MMS_SERVICE_GET_NAMED_VARIABLE_LIST_ATTRIBUTES 0x08
#define MMS_SERVICE_DELETE_NAMED_VARIABLE_LIST 0x04
#define MMS_SERVICE_DEFINE_NAMED_TYPE 0x02
#define MMS_SERVICE_GET_NAMED_TYPE_ATTRIBUTES 0x01 */

// Constructor-destructor pair
MmsServerConnection *MmsServerConnection_init( MmsServerConnection *, MmsServer);
void MmsServerConnection_destroy(MmsServerConnection *);

// Other
/* MmsIndication MmsServerConnection_parseMessage( MmsServerConnection *, 
                                                ByteBuffer *, SBuffer *);
bool MmsServerConnection_addNamedVariableList( MmsServerConnection *,
                                               MmsNamedVariableList);
MmsNamedVariableList MmsServerConnection_getNamedVariableList(MmsServerConnection *, char *);
LinkedList MmsServerConnection_getNamedVariableLists(MmsServerConnection*);
void MmsServerConnection_deleteNamedVariableList(MmsServerConnection *, char *);
char *MmsServerConnection_getClientAddress(MmsServerConnection *); */

#endif /* __MMS_SERVER_CONNECTION_H__ */
