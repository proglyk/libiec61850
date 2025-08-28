#include "libiec61850/AcseConnection.h"
#include <stdlib.h>
#include "iso8650/ACSEapdu.h"

// Type definitions

struct sAcseConnection {
  // Own needs
	AcseConnectionState	 state;
	s32_t	           		 nextReference;
	AcseAuthenticationParameter authentication;
  // Layer Buffer
  u8_t                *userDataBuffer;
	s32_t                userDataBufferSize;
  ByteBuffer           mmsInitRequest;
  // Linkage with the upper layer
  MmsServerConnection *mmsConn;
  SBufferPtr           sbuf;
  PassedHandlerPtr     msgPassedHandler;
  void                *msgPassedParam;
  
};

static AcseIndication parseMessage(AcseConnectionPtr, ByteBuffer *);
static AcseIndication createSAssociateResponseMessage( AcseConnectionPtr,	
                                                       SBuffer * );
static AcseIndication parseAarqPdu(AcseConnectionPtr, AARQapdu_t *);
static void	writer(asn_TYPE_descriptor_t *, ACSEapdu_t*, SBuffer *);

static uint8_t appContextNameMms[] = {0x28, 0xca, 0x22, 0x02, 0x03};
static uint8_t berOid[2] = {0x51, 0x01};
static asn_enc_rval_t rval;

/**	----------------------------------------------------------------------------
	* @brief Acse Connection layer constructor */
AcseConnectionPtr
	AcseConnection_Create(SBufferPtr sbuf, void *pld) {
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
  // Top layers creating
  self->mmsConn = MmsServerConnection_init(pld/* mmsServer */);
  if (!self->mmsConn) return NULL;
  //Map_addEntry(mmsServer->openConnections, NULL, self->mmsConn);
  
  return self;
}

/**	----------------------------------------------------------------------------
	* @brief Acse Connection layer destructor */
void
  AcseConnection_Delete(AcseConnectionPtr self) {
/*----------------------------------------------------------------------------*/
	if (!self) return;
  if (self->mmsConn) {
    //MmsServer_Deinit(self->mmsServ);
    //MmsServer_destroy(self->mmsServ);
    //MmsServerConnection_down(self->mmsConn);
    MmsServerConnection_destroy(self->mmsConn);
  }
	free(self); self = NULL;
}

/**	----------------------------------------------------------------------------
	* @brief Connection init */
MmsServerConnection *
	AcseConnection_getMmsConn( AcseConnectionPtr self ) {
/*----------------------------------------------------------------------------*/
	return self->mmsConn;
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
	//if (!self->msgPassedHandler) return -1;
  // ???
  AcseIndication sta = parseMessage(self, buffer);
  if (sta != ACSE_ASSOCIATE) return -1;
  
  //
  ByteBuffer_wrap( &self->mmsInitRequest, self->userDataBuffer,
                    self->userDataBufferSize, self->userDataBufferSize );
  //self->msgPassedHandler( self->msgPassedParam, &self->mmsInitRequest, self->sbuf );
  MmsServerConnection_parseMessage( self->mmsConn, &self->mmsInitRequest, 
                                    self->sbuf );
  
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

	assocInfo->list.array[0]->indirectreference = &(self->nextReference);

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

static AcseIndication
parseAarqPdu(AcseConnectionPtr self, AARQapdu_t* aarq)
{
	//if (checkAuthentication(self, aarq) == false)
	//	return ACSE_ASSOCIATE_FAILED;

	if (aarq->userinformation->list.count == 1) {
		Myexternal_t* external = aarq->userinformation->list.array[0];

		if (external->indirectreference == 0)
			return ACSE_ERROR;
		else {
			if (external->encoding.present != encoding_PR_singleASN1type)
				return ACSE_ERROR;
			else {
				uint8_t* userDataBuffer = external->encoding.choice.singleASN1type.buf;
				int userDataBufferSize = external->encoding.choice.singleASN1type.size;

				self->userDataBufferSize = userDataBufferSize;

				//if (self->userDataBuffer != NULL)
				//	free(self->userDataBuffer);

				//self->userDataBuffer = malloc(userDataBufferSize);
				memcpy(self->userDataBuffer, userDataBuffer, userDataBufferSize);
			}
			self->nextReference = *(external->indirectreference);
		}

		return ACSE_ASSOCIATE;
	}
	else
		return ACSE_ERROR;
}

static void
	writer(asn_TYPE_descriptor_t * pxType, ACSEapdu_t* pxApdu, SBuffer * pxSbuf) {
	
	//pxSbuf->ulFrontLenght = 0;
	der_encode(pxType, pxApdu, (asn_app_consume_bytes_f *)SBuffer_Writer, (void*) pxSbuf);
	
	// т.к der_encode записывает в начало массива помимо заголовка acse еще и
	// содержимое mms, которое уже находилось в массиве в конце, то нужно просто
	// представить, что массив пустой и толкать данные в самый конец, затирая пред.
	
	SBuffer_Replace(pxSbuf);
	//pxSbuf->ulSize = 0;
	//pxSbuf->ulSpace = pxSbuf->ulMaxSize;
	//SBuffer_Shift(pxSbuf, pxSbuf->ulFrontLenght);
}
