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
  PassedHandlerPtr    msgPassedHandler;
  void               *msgPassedParam;
  
};

static AcseIndication parseMessage(AcseConnectionPtr, ByteBuffer *);
static AcseIndication createSAssociateResponseMessage( AcseConnectionPtr,	
                                                       SBuffer * );

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
                                  PassedHandlerPtr handler,
                                  void* param )
{
/*----------------------------------------------------------------------------*/
	self->msgPassedHandler = handler;
	self->msgPassedParam = param;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
s32_t
  AcseConnection_Connect(AcseConnectionPtr self, ByteBuffer *buffer) {
/*----------------------------------------------------------------------------*/
	if (!self->msgPassedHandler) return -1;
  // ???
  AcseIndication sta = parseMessage(self, buffer);
  if (sta != ACSE_ASSOCIATE) return -1;
  
  //
  ByteBuffer_wrap( &self->mmsInitRequest, self->userDataBuffer,
                    self->userDataBufferSize, self->userDataBufferSize );
  self->msgPassedHandler( self->msgPassedParam, &self->mmsInitRequest, self->sbuf );
  if (SBuffer_GetPayloadSize(self->sbuf) <= 0) return -1;
  
  // ответ
  createSAssociateResponseMessage(self, self->sbuf);
  
  return 0;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
AcseIndication
  parseMessage(AcseConnectionPtr self, ByteBuffer *message) {
/*----------------------------------------------------------------------------*/
	ACSEapdu_t* acseApdu = 0;
	AcseIndication indication;
	asn_dec_rval_t rval;

	/* Decoder return value  */
	rval = ber_decode(NULL, &asn_DEF_ACSEapdu, (void**) &acseApdu,
		(void*) message->buffer, message->size);

	/* print message XER encoded (XML) */
//	if (ISO8650_DEBUG) xer_fprint(stdout, &asn_DEF_ACSEapdu, acseApdu);

	if (acseApdu->present == ACSEapdu_PR_aarq) {
		indication = parseAarqPdu(self, &(acseApdu->choice.aarq));
	}
	else if (acseApdu->present == ACSEapdu_PR_aare) {
		//indication = parseAarePdu(self, &(acseApdu->choice.aare));
	}
	else
		indication = ACSE_ERROR;

	asn_DEF_ACSEapdu.free_struct(&asn_DEF_ACSEapdu, acseApdu, 0);

	return indication;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static AcseIndication
  createSAssociateResponseMessage(AcseConnectionPtr self, SBuffer* payload) {
/*----------------------------------------------------------------------------*/
	ACSEapdu_t* pdu = calloc(1, sizeof(ACSEapdu_t));

	pdu->present = ACSEapdu_PR_aare;

	pdu->choice.aare.applicationcontextname.buf = appContextNameMms;
	pdu->choice.aare.applicationcontextname.size = 5;

	pdu->choice.aare.result = Associateresult_accepted;

	pdu->choice.aare.resultsourcediagnostic.present =
			Associatesourcediagnostic_PR_acseserviceuser;

	pdu->choice.aare.resultsourcediagnostic.choice.acseserviceuser =
			acseserviceuser_null;

	Associationinformation_t* assocInfo =
			calloc(1, sizeof(Associationinformation_t));

	assocInfo->list.count = 1;
	assocInfo->list.array = calloc(1, sizeof(struct MyExternal*));
	assocInfo->list.array[0] = calloc(1, sizeof(struct Myexternal));

	assocInfo->list.array[0]->directreference = calloc(1, sizeof(OBJECT_IDENTIFIER_t));
	assocInfo->list.array[0]->directreference->size = 2;
	assocInfo->list.array[0]->directreference->buf = berOid;

	assocInfo->list.array[0]->indirectreference = &(connection->nextReference);

	assocInfo->list.array[0]->encoding.present = encoding_PR_singleASN1type;
	assocInfo->list.array[0]->encoding.choice.singleASN1type.size = 
		SBuffer_GetPayloadSize(payload);
	assocInfo->list.array[0]->encoding.choice.singleASN1type.buf = 
		SBuffer_GetPayload(payload);


	pdu->choice.aare.userinformation = assocInfo;
	
	writer(&asn_DEF_ACSEapdu, pdu, payload);
	

	/* free data structure */
	free(assocInfo->list.array[0]->directreference);
	free(assocInfo->list.array[0]);
	free(assocInfo->list.array);
	free(assocInfo);
	free(pdu);

	return rval.encoded;
}