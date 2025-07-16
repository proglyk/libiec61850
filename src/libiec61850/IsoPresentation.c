#include "libiec61850/IsoPresentation.h"
#include "libiec61850/AcseConnection.h"
#include "libiec61850/byte_buffer.h"

struct sIsoPresentation {
  // локальные
	u32_t             callingPresentationSelector;
	u32_t             calledPresentationSelector;
	ContextDefinition context[2];
	u8_t              nextContextId;
	ByteBuffer        nextPayload;
  // связь с уровнями
  AcseConnectionPtr acseConnection;
};

/**	----------------------------------------------------------------------------
	* @brief ??? */
IsoPresentationPtr
	IsoPresentation_Create(void) {
/*----------------------------------------------------------------------------*/
	IsoPresentationPtr self = calloc(1, sizeof(struct sIsoPresentation));
  if (!self) return NULL;
  
  return self;
}