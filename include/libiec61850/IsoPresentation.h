#ifndef _ISOPRESENTATION_H_
#define _ISOPRESENTATION_H_

#include "userint.h"

// Type declarations

typedef struct {
	uint8_t contextId;
	uint8_t abstractSyntaxName[10]; /* contains OID */
} ContextDefinition;

struct sIsoPresentation;
typedef struct sIsoPresentation *IsoPresentationPtr;

// Function declarations

IsoPresentationPtr IsoPresentation_Create(SBufferPtr);
void               IsoPresentation_Delete(IsoPresentationPtr);

#endif // _ISOPRESENTATION_H_