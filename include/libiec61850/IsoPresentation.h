#ifndef _ISOPRESENTATION_H_
#define _ISOPRESENTATION_H_

#include "userint.h"
#include "libiec61850/utils/SBuffer.h"
#include "libiec61850/utils/common_types.h"

// Type declarations

typedef struct {
	uint8_t contextId;
	uint8_t abstractSyntaxName[10]; /* contains OID */
} CtxDef;

typedef enum {
	PRESENTATION_OK,
	PRESENTATION_ERROR
} IsoPresStatus;

struct sIsoPresentation;
typedef struct sIsoPresentation *IsoPresPtr;

// Function declarations

IsoPresPtr IsoPresentation_Create(SBufferPtr, void *);
void       IsoPresentation_Delete(IsoPresPtr);
s32_t      IsoPresentation_Connect(IsoPresPtr, ByteBuffer *);
s32_t      IsoPresentation_ProcessData(IsoPresPtr, ByteBuffer *);
void       IsoPresentation_InstallListener(IsoPresPtr, PassedHandlerPtr, void *);
void       IsoPresentation_ThrowOverListener(IsoPresPtr, PassedHandlerPtr, void *);

#endif // _ISOPRESENTATION_H_