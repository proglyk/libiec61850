#ifndef _ISOSESSION_H_
#define _ISOSESSION_H_

#include "userint.h"
#include "libiec61850/byte_buffer.h"
#include "libiec61850/SBuffer.h"

// Type declarations

typedef enum {
	SESSION_OK,	SESSION_ERROR, SESSION_CONNECT,	SESSION_GIVE_TOKEN,	SESSION_DATA
} IsoSessionIndication;

struct sIsoSession;
typedef struct sIsoSession *IsoSessionPtr;

// Function declarations

IsoSessionPtr IsoSession_Create(SBufferPtr);
void          IsoSession_Delete(IsoSessionPtr);
s32_t         IsoSession_Do(IsoSessionPtr, ByteBuffer *buf);

#endif // _ISOSESSION_H_