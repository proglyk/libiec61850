#include "libiec61850/IsoPresentation.h"
#include "libiec61850/AcseConnection.h"
#include "libiec61850/byte_buffer.h"

// Type definitions

struct sIsoPresentation {
  // Own needs
	u32_t             callingPresentationSelector;
	u32_t             calledPresentationSelector;
	ContextDefinition context[2];
	u8_t              nextContextId;
	ByteBuffer        nextPayload;
  // Linkage with the upper layer
  AcseConnectionPtr acseConn;
  SBufferPtr        sbuf;
};

/**	----------------------------------------------------------------------------
	* @brief Iso Presentation layer constructor */
IsoPresentationPtr
	IsoPresentation_Create(SBufferPtr sbuf) {
/*----------------------------------------------------------------------------*/
	// Self creating
  IsoPresentationPtr self = calloc(1, sizeof(struct sIsoPresentation));
  if (!self) return NULL;
  // линкуем SBuffer
  self->sbuf = sbuf;
  // Top layers creating
  self->acseConn = AcseConnection_Create(self->sbuf);
  if (!self->acseConn) return NULL;
  
  return self;
}

/**	----------------------------------------------------------------------------
	* @brief Iso Presentation layer destructor */
void
  IsoPresentation_Delete(IsoPresentationPtr self) {
/*----------------------------------------------------------------------------*/
	if (!self) return;
  if (self->acseConn) AcseConnection_Delete(self->acseConn);
	free(self); self = NULL;
}