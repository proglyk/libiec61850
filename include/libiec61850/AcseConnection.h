#ifndef _ACSECONNECTION_H_
#define _ACSECONNECTION_H_

#include "userint.h"
#include "libiec61850/utils/SBuffer.h"
#include "libiec61850/utils/common_types.h"
#include "libiec61850/mms/mms_server_conn.h"

// Type declarations

typedef enum eAcseIndication {
	ACSE_ERROR, ACSE_ASSOCIATE, ACSE_ASSOCIATE_FAILED, ACSE_OK
} AcseIndication;

typedef enum {
	AUTH_NONE,
	AUTH_PASSWORD
} AcseAuthenticationMechanism;

typedef struct sAcseAuthenticationParameter {
	AcseAuthenticationMechanism mechanism;
	union {
		struct {
			char* string;
		} password;
	} value;
}* AcseAuthenticationParameter;

typedef enum eAcseConnectionState {
	idle, requestIndicated, connected
} AcseConnectionState;

struct sAcseConnection;
typedef struct sAcseConnection *AcseConnectionPtr;

// Function declarations

AcseConnectionPtr AcseConnection_Create(SBufferPtr);
void              AcseConnection_Delete(AcseConnectionPtr);
s32_t             AcseConnection_Connect(AcseConnectionPtr, ByteBuffer *);
void              AcseConnection_InstallListener(AcseConnectionPtr, PassedHandlerPtr, void *);
MmsServerConnection *AcseConnection_getMmsConn( AcseConnectionPtr self );

#endif // _ACSECONNECTION_H_