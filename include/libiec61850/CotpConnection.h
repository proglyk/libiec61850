#ifndef _COTPCONNECTION_H_
#define _COTPCONNECTION_H_

#include "userint.h"
#include "libiec61850/utils/byte_buffer.h"
#include "libiec61850/utils/SBuffer.h"
#include "libiec61850/utils/common_types.h"

// Type declarations

typedef enum {
	COTP_CON_RUN, COTP_CON_STOP
} CotpState;

typedef enum {
	COTP_OK=0, COTP_ERROR, COTP_CONNECT_INDICATION, COTP_DATA_INDICATION,
	COTP_DISCONNECT_INDICATION
} CotpIndication;

typedef struct {
	int32_t tsap_id_src;
	int32_t tsap_id_dst;
	uint8_t tpdu_size;
} CotpOptions;

struct sCotpConnection;
typedef struct sCotpConnection *CotpConnectionPtr;

// Function declarations

CotpConnectionPtr CotpConnection_Create(s32_t, ByteBuffer *, SBufferPtr);
void              CotpConnection_Delete(CotpConnectionPtr);
s32_t             CotpConnection_Do(CotpConnectionPtr);
void	            CotpConnection_ThrowOverListener(CotpConnectionPtr, MsgPassedHandlerPtr, void *);

#endif // _COTPCONNECTION_H_