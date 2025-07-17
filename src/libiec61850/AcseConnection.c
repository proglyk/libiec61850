#include "libiec61850/AcseConnection.h"
#include <stdlib.h>

// Type definitions

struct sAcseConnection {
  // Own needs
	AcseConnectionState	state;
	s32_t	           		nextReference;
	uint8_t* 				    userDataBuffer;
	s32_t               userDataBufferSize;
	AcseAuthenticationParameter authentication;
  // Linkage with the upper layer
  SBufferPtr          sbuf;
};

/**	----------------------------------------------------------------------------
	* @brief Acse Connection layer constructor */
AcseConnectionPtr
	AcseConnection_Create(SBufferPtr sbuf) {
/*----------------------------------------------------------------------------*/
	// Self creating
  AcseConnectionPtr self = calloc(1, sizeof(struct sAcseConnection));
  if (!self) return NULL;
  // Self configurating
  self->state = idle;
	self->nextReference = 0;
	self->userDataBuffer = calloc(40, sizeof(u8_t));
	self->userDataBufferSize = 0;
	self->authentication = NULL;
  self->sbuf = sbuf;
  
  return self;
}

/**	----------------------------------------------------------------------------
	* @brief Acse Connection layer destructor */
void
  AcseConnection_Delete(AcseConnectionPtr self) {
/*----------------------------------------------------------------------------*/
	if (!self) return;
	free(self); self = NULL;
}