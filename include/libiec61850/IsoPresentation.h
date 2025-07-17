#ifndef _ISOPRESENTATION_H_
#define _ISOPRESENTATION_H_

#include "userint.h"
#include "libiec61850/utils/SBuffer.h"
#include "libiec61850/utils/common_types.h"

// Type declarations

typedef struct {
	uint8_t contextId;
	uint8_t abstractSyntaxName[10]; /* contains OID */
} ContextDefinition;

typedef enum {
	PRESENTATION_OK,
	PRESENTATION_ERROR
} IsoPresStatus;

struct sIsoPresentation;
typedef struct sIsoPresentation *IsoPresentationPtr;

// Function declarations

IsoPresentationPtr IsoPresentation_Create(SBufferPtr);
void               IsoPresentation_Delete(IsoPresentationPtr);
s32_t	             IsoPresentation_parseConnect(IsoPresentationPtr, ByteBuffer *);
void               IsoPresentation_ThrowOverListener(IsoPresentationPtr, MsgPassedHandlerPtr, void *);

#endif // _ISOPRESENTATION_H_