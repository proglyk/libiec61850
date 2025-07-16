#include "libiec61850/IsoSession.h"
#include "libiec61850/IsoPresentation.h"
#include "libiec61850/byte_buffer.h"

struct sIsoSession {
  // локальные
  u16_t callingSessionSelector;
	u16_t calledSessionSelector;
	u16_t sessionRequirement;
	u8_t protocolOptions;
	ByteBuffer userData;
  // связь с уровнями
  IsoPresentationPtr isoPresentation;
};

/**	----------------------------------------------------------------------------
	* @brief ??? */
IsoSessionPtr
	IsoSession_Create(void) {
/*----------------------------------------------------------------------------*/
	IsoSessionPtr self = calloc(1, sizeof(struct sIsoSession));
  if (!self) return NULL;
  
  return self;
}