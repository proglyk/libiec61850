#ifndef _ACSECONNECTION_H_
#define _ACSECONNECTION_H_

#include "userint.h"

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

AcseConnectionPtr AcseConnection_Create(void);
void              AcseConnection_Delete(AcseConnectionPtr);

#endif // _ACSECONNECTION_H_