#ifndef _ISOCONNECTION_H_
#define _ISOCONNECTION_H_

#include "userint.h"

struct sIsoConnection;
typedef struct sIsoConnection *IsoConnectionPtr;

IsoConnectionPtr IsoConnection_Create(s32_t);
void	           IsoConnection_Delete(IsoConnectionPtr);
void	           IsoConnection_ClientConnected(IsoConnectionPtr);

#endif // _ISOCONNECTION_H_