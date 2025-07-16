#include "libiec61850/IsoSession.h"
#include "libiec61850/IsoPresentation.h"
#include "libiec61850/byte_buffer.h"

struct sIsoSession {
  // локальные
  u16_t      callingSessionSelector;
	u16_t      calledSessionSelector;
	u16_t      sessionRequirement;
	u8_t       protocolOptions;
	ByteBuffer userData;
  // Top layers linkage
  IsoPresentationPtr isoPresent;
};

/**	----------------------------------------------------------------------------
	* @brief Iso Session layer constructor */
IsoSessionPtr
	IsoSession_Create(void) {
/*----------------------------------------------------------------------------*/
	// Self creating
  IsoSessionPtr self = calloc(1, sizeof(struct sIsoSession));
  if (!self) return NULL;
  // Top layers creating
  self->isoPresent = IsoPresentation_Create();
  if (!self->isoPresent) return NULL;
  
  return self;
}

/**	----------------------------------------------------------------------------
	* @brief Iso Session layer destructor */
void
  IsoSession_Delete(IsoSessionPtr self) {
/*----------------------------------------------------------------------------*/
	if (!self) return;
  if (self->isoPresent) IsoPresentation_Delete(self->isoPresent);
	free(self); self = NULL;
}