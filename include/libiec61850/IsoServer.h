#ifndef _ISOSERVER_H_
#define _ISOSERVER_H_

#include "libiec61850/IsoConnection.h"

typedef enum {
	ISO_SVR_STATE_IDLE,
	ISO_SVR_STATE_WAITCON,
	ISO_SVR_STATE_RUNNING,
	ISO_SVR_STATE_STOPPED,
	ISO_SVR_STATE_ERROR
} IsoServerState;

typedef enum {
	ISO_CONNECTION_OPENED,
	ISO_CONNECTION_CLOSED
} IsoConnectionIndication;

typedef void (*ConnectionIndicationHandler) ( IsoConnectionIndication, 
                                              void*,
                                              IsoConnectionPtr );

struct sIsoServer;
typedef struct sIsoServer *IsoServerPtr;

IsoServerPtr IsoServer_Create(void);
void         IsoServer_Delete(IsoServerPtr);
void         IsoServer_Listen(IsoServerPtr);
void	       IsoServer_setConnectionHandler( IsoServerPtr,
                                             ConnectionIndicationHandler,
                                             void* );

#endif // _ISOSERVER_H_