#ifndef _ISOCONNECTION_H_
#define _ISOCONNECTION_H_

#include "userint.h"
#include "libiec61850/utils/byte_buffer.h"
#include "libiec61850/utils/SBuffer.h"
#include "libiec61850/utils/common_types.h"

#define ISO_CON_STATE_ERROR		2
#define ISO_CON_STATE_RUNNING 1
#define ISO_CON_STATE_STOPPED 0

// Type declarations

struct sIsoConnection;
typedef struct sIsoConnection *IsoConnectionPtr;

//typedef void (*MessageReceivedHandler) (void *, ByteBuffer *, SBuffer *);

// Function declarations

IsoConnectionPtr IsoConnection_Create(s32_t);
s32_t            IsoConnection_Init(IsoConnectionPtr);
void	           IsoConnection_Deinit(IsoConnectionPtr);
void	           IsoConnection_Delete(IsoConnectionPtr);
s32_t	           IsoConnection_ClientConnected(IsoConnectionPtr);
void             IsoConnection_InstallListener( IsoConnectionPtr,
                                                MsgPassedHandlerPtr,
                                                void* );

#endif // _ISOCONNECTION_H_