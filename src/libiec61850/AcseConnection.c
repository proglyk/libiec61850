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
	* @brief ??? */
AcseConnectionPtr
	AcseConnection_Create(void) {
/*----------------------------------------------------------------------------*/
	AcseConnectionPtr self = calloc(1, sizeof(struct sAcseConnection));
  if (!self) return NULL;
  
  return self;
}