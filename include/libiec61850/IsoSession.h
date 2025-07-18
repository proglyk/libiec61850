#ifndef _ISOSESSION_H_
#define _ISOSESSION_H_

#include "userint.h"
#include "libiec61850/utils/byte_buffer.h"
#include "libiec61850/utils/SBuffer.h"
#include "libiec61850/utils/common_types.h"

// Type declarations

typedef enum {
	SESSION_OK,	SESSION_ERROR, SESSION_CONNECT,	SESSION_GIVE_TOKEN,	SESSION_DATA
} IsoSessionIndication;

typedef enum {
  SESS_REQ_CONNECT, SESS_REQ_DATA
} IsoSessRequestType;

struct sIsoSession;
typedef struct sIsoSession *IsoSessionPtr;

// Function declarations

IsoSessionPtr IsoSession_Create(SBufferPtr);
void          IsoSession_Delete(IsoSessionPtr);
s32_t         IsoSession_Process(IsoSessionPtr, ByteBuffer *buf, IsoSessRequestType);
void          IsoSession_ThrowOverListener(IsoSessionPtr, MsgPassedHandlerPtr, void *);

#endif // _ISOSESSION_H_