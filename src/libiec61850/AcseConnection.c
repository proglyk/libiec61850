#include "libiec61850/AcseConnection.h"
#include "stdlib.h"

struct sAcseConnection {
  // локальные
	AcseConnectionState	state;
	s32_t	           		nextReference;
	uint8_t* 				    userDataBuffer;
	s32_t               userDataBufferSize;
	AcseAuthenticationParameter authentication;
  // связь с уровнями
};

/**	----------------------------------------------------------------------------
	* @brief Acse Connection layer constructor */
AcseConnectionPtr
	AcseConnection_Create(void) {
/*----------------------------------------------------------------------------*/
	// Self creating
  AcseConnectionPtr self = calloc(1, sizeof(struct sAcseConnection));
  if (!self) return NULL;
  // Self configurating
  self->state = idle;
	self->nextReference = 0;
	self->userDataBuffer = NULL;
	self->userDataBufferSize = 0;
	self->authentication = NULL;
  
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