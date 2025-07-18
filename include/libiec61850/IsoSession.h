#ifndef _ISOSESSION_H_
#define _ISOSESSION_H_

#include "userint.h"
#include "libiec61850/utils/byte_buffer.h"
#include "libiec61850/utils/SBuffer.h"
#include "libiec61850/utils/common_types.h"

// Type declarations

typedef enum {
	SESSION_OK,	SESSION_ERROR, SESSION_CONNECT,	SESSION_GIVE_TOKEN,	SESSION_DATA
} IsoSessStatus;

struct sIsoSession;
typedef struct sIsoSession *IsoSessPtr;

// Function declarations

IsoSessPtr IsoSession_Create(SBufferPtr);
void       IsoSession_Delete(IsoSessPtr);
s32_t      IsoSession_Process(IsoSessPtr, ByteBuffer *);
void       IsoSession_ThrowOverListener(IsoSessPtr, PassedHandlerPtr, void *);

#endif // _ISOSESSION_H_