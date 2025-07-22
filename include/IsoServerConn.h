#ifndef   _ISOSERVERCONN_H_
#define   _ISOSERVERCONN_H_

#include "libiec61850/IsoServer.h"

typedef struct {
  IndicationHandler pvMmsHandler;
  void             *pvMmsPayload;
} IsoServerPld_t;

#endif // _ISOSERVERCONN_H_
