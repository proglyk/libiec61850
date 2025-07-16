#ifndef _ISOPRESENTATION_H_
#define _ISOPRESENTATION_H_

#include "userint.h"

typedef struct {
	uint8_t contextId;
	uint8_t abstractSyntaxName[10]; /* contains OID */
} ContextDefinition;

struct sIsoPresentation;
typedef struct sIsoPresentation *IsoPresentationPtr;

IsoPresentationPtr IsoPresentation_Create(void);

#endif // _ISOPRESENTATION_H_