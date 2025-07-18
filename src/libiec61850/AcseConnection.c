#include "libiec61850/AcseConnection.h"
#include <stdlib.h>

// Type definitions

struct sAcseConnection {
  // Own needs
	AcseConnectionState	state;
	s32_t	           		nextReference;
	AcseAuthenticationParameter authentication;
  // Layer Buffer
  u8_t               *userDataBuffer;
	s32_t               userDataBufferSize;
  ByteBuffer          mmsInitRequest;
  // Linkage with the upper layer
  SBufferPtr          sbuf;
  MsgPassedHandlerPtr msgPassedHandler;
  void               *msgPassedParam;
  
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
  self->msgPassedHandler = NULL;
  self->msgPassedParam = NULL;
  
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

/**	----------------------------------------------------------------------------
	* @brief Connection init */
void
	AcseConnection_InstallListener( AcseConnectionPtr self,
                                  MsgPassedHandlerPtr handler,
                                  void* param )
{
/*----------------------------------------------------------------------------*/
	self->msgPassedHandler = handler;
	self->msgPassedParam = param;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
s32_t
  AcseConnection_parseMessage(AcseConnectionPtr self, ByteBuffer *message) {
/*----------------------------------------------------------------------------*/
  AcseIndication indication = ACSE_ASSOCIATE_FAILED;
/* 	ACSEapdu_t* acseApdu = 0;
	asn_dec_rval_t rval;

	// Decoder return value
	rval = ber_decode(NULL, &asn_DEF_ACSEapdu, (void**) &acseApdu,
		(void*) message->buffer, message->size);

// print message XER encoded (XML)
//	if (ISO8650_DEBUG) xer_fprint(stdout, &asn_DEF_ACSEapdu, acseApdu);

	if (acseApdu->present == ACSEapdu_PR_aarq) {
		indication = parseAarqPdu(self, &(acseApdu->choice.aarq));
	}
	else if (acseApdu->present == ACSEapdu_PR_aare) {
		//indication = parseAarePdu(self, &(acseApdu->choice.aare));
	}
	else
		indication = ACSE_ERROR;
	asn_DEF_ACSEapdu.free_struct(&asn_DEF_ACSEapdu, acseApdu, 0); */
  
  //
  if (indication != ACSE_ASSOCIATE) return -1;
  
  //
  if (self->msgPassedHandler) {
      //
    ByteBuffer_wrap( &self->mmsInitRequest,
                     self->userDataBuffer,
                     self->userDataBufferSize,
                     self->userDataBufferSize );
    //
    self->msgPassedHandler( self->msgPassedParam,
                            &self->mmsInitRequest,
                            self->sbuf );
  }
  
	return 0;
}
