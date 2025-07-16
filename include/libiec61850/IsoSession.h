#ifndef _ISOSESSION_H_
#define _ISOSESSION_H_

#include "userint.h"

struct sIsoSession;
typedef struct sIsoSession *IsoSessionPtr;

IsoSessionPtr IsoSession_Create(void);

#endif // _ISOSESSION_H_