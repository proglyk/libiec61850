#include "stack_config.h"
#include "libiec61850/mms/mms_server_conn.h"
#include "iso9506/MmsPdu.h"
#include "libiec61850/mms/mms_server_common.h"
#include "libiec61850/mms/mms_service_read.h"
#include "libiec61850/mms/mms_service_write.h"
#include "libiec61850/mms/mms_service_named_variable_list.h"

extern void mmsServer_handleGetNameListRequest(	MmsServerConnection* connection,
	GetNameListRequest_t* getNameList, int invokeId, ByteBuffer* response);
extern int mmsServer_handleGetVariableAccessAttributesRequest(MmsServerConnection *,
  GetVariableAccessAttributesRequest_t *, int, ByteBuffer *);

static MmsIndication parseMmsPdu(MmsServerConnection *, ByteBuffer *, SBuffer*);
static int handleInitiateRequestPdu(MmsServerConnection *, InitiateRequestPdu_t *, ByteBuffer *);
static void parseInitiateRequestPdu(MmsServerConnection *, const InitiateRequestPdu_t *);
static int createInitiateResponse(MmsServerConnection *, ByteBuffer *);
static InitiateResponsePdu_t createInitiateResponsePdu(MmsServerConnection *);
static int handleConfirmedRequestPdu( MmsServerConnection*,	ConfirmedRequestPdu_t*, ByteBuffer*);
static void	freeInitiateResponsePdu(InitiateResponsePdu_t);

static uint8_t servicesSupported[] = {
  MMS_SERVICE_GET_NAME_LIST | MMS_SERVICE_READ | MMS_SERVICE_WRITE | 
    MMS_SERVICE_GET_VARIABLE_ACCESS_ATTRIBUTES | 
    MMS_SERVICE_GET_NAMED_VARIABLE_LIST_ATTRIBUTES,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Public 

/**	----------------------------------------------------------------------------
	* @brief MMS server conn layer constructor */
MmsServerConnection*
  MmsServerConnection_init( void *pld/* , IsoConnection isoCon */ ) {
/*----------------------------------------------------------------------------*/
	MmsServerConnection* self = calloc(1, sizeof(MmsServerConnection));

	self->maxServOutstandingCalled = 0;
	self->maxServOutstandingCalling = 0;
	self->maxPduSize = MMS_MAXIMUM_PDU_SIZE;
	self->dataStructureNestingLevel = 0;
	self->server = (MmsServer)pld;
	//self->isoConnection = isoCon;
	self->namedVariableLists = LinkedList_create();

	//IsoConnection_installListener(isoCon, messageReceived, (void*) self);
  
  //TODO "заменить NUUL на mmsServer"
  MmsServer_conn_opened((MmsServer)pld, (void *)self);

	return self;
}

/**	----------------------------------------------------------------------------
	* @brief MMS server conn layer destructor */
void
  MmsServerConnection_destroy(MmsServerConnection* self) {
/*----------------------------------------------------------------------------*/
	MmsServer_conn_closed(NULL, (void *)self); // TODO заменить NULL
  LinkedList_destroyDeep( self->namedVariableLists,
                          (void (*)(void *))MmsNamedVariableList_destroy );
	free(self);
}

/**	----------------------------------------------------------------------------
	* @brief Acse Connection layer constructor */
MmsIndication
	MmsServerConnection_parseMessage( MmsServerConnection* self,
                                    ByteBuffer* message, SBuffer* response) {
/*----------------------------------------------------------------------------*/
	return parseMmsPdu(self, message, response);
}

LinkedList
	MmsServerConnection_getNamedVariableLists(MmsServerConnection* self){
	return self->namedVariableLists;
}

// Static 

/**	----------------------------------------------------------------------------
	* @brief Acse Connection layer constructor */
static MmsIndication
  parseMmsPdu( MmsServerConnection* self, ByteBuffer* message, SBuffer* response) {
/*----------------------------------------------------------------------------*/
	MmsPdu_t* mmsPdu = 0; /* allow asn1c to allocate structure */
	MmsIndication retVal;

	asn_dec_rval_t rval;
	/* Decoder return value  */
	rval = ber_decode(NULL, &asn_DEF_MmsPdu,
			(void**) &mmsPdu, (void*) message->buffer, message->size);

	//if (DEBUG) xer_fprint(stdout, &asn_DEF_MmsPdu, mmsPdu);

	if (rval.code != RC_OK) {
		retVal = MMS_ERROR;
		goto parseMmsPdu_exit;
	}

	switch (mmsPdu->present) {
	case MmsPdu_PR_initiateRequestPdu:
		//LWIP_DEBUGF(ISO9506_DEBUG, ("-----ISO9506: Initiate Request OK\r\n"));
		handleInitiateRequestPdu(self, &(mmsPdu->choice.initiateRequestPdu), 
			(ByteBuffer*)response);
		retVal = MMS_INITIATE;
		break;
	case MmsPdu_PR_confirmedRequestPdu:
		// отключил дл¤ реализации в данном виде (старый server_parts и ied_server,
		// когда ¤ пыталс¤ идти снизу вверх.)
		// —мотреть папку "ied", где исп. подход сверху вниз
		//LWIP_DEBUGF(ISO9506_DEBUG, ("-----ISO9506: Confirmed Request OK\r\n"));
		handleConfirmedRequestPdu(self, &(mmsPdu->choice.confirmedRequestPdu), 
			(ByteBuffer*)response);
		retVal = MMS_CONFIRMED_REQUEST;
		break;
	case MmsPdu_PR_concludeRequestPDU:
		//mmsServer_writeConcludeResponsePdu(response);
		//IsoConnection_close(self->isoConnection);
		retVal = MMS_CONCLUDE;
		break;
	default:
		//writeMmsRejectPdu(NULL, REJECT_UNKNOWN_PDU_TYPE, response);
		retVal = MMS_ERROR;
		break;
	}
	
	SBuffer_Shift(response);
	//SBuffer_Shift(response, response->ulFrontLenght);
		

parseMmsPdu_exit:
	asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);
	return retVal;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static int
  handleInitiateRequestPdu( MmsServerConnection* self, InitiateRequestPdu_t* request, ByteBuffer* response) {
/*----------------------------------------------------------------------------*/
	parseInitiateRequestPdu(self, request);

	//TODO here we should indicate if something is wrong!
	createInitiateResponse(self, response);

	return 0;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
void
  parseInitiateRequestPdu(MmsServerConnection* self, const InitiateRequestPdu_t* request) {
/*----------------------------------------------------------------------------*/
	self->maxServOutstandingCalled =
			request->proposedMaxServOutstandingCalled;
	self->maxServOutstandingCalling =
			request->proposedMaxServOutstandingCalling;

	if (request->localDetailCalling == 0) {
		self->maxPduSize = MMS_MAXIMUM_PDU_SIZE;
	}
	else {
		if (*(request->localDetailCalling) > MMS_MAXIMUM_PDU_SIZE)
			self->maxPduSize = MMS_MAXIMUM_PDU_SIZE;
		else
			self->maxPduSize = *(request->localDetailCalling);
	}

	if (request->proposedDataStructureNestingLevel == 0)
		self->dataStructureNestingLevel =
				DEFAULT_DATA_STRUCTURE_NESTING_LEVEL;
	else
		self->dataStructureNestingLevel =
				*(request->proposedDataStructureNestingLevel);
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static int
  createInitiateResponse(MmsServerConnection* self, ByteBuffer* writeBuffer) {
/*----------------------------------------------------------------------------*/
	MmsPdu_t* mmsPdu = calloc(1, sizeof(MmsPdu_t));

	mmsPdu->present = MmsPdu_PR_initiateResponsePdu;

	mmsPdu->choice.initiateResponsePdu = createInitiateResponsePdu(self);

	asn_enc_rval_t rval;

	rval = der_encode(&asn_DEF_MmsPdu, mmsPdu, mmsServer_write_out, (void*) writeBuffer);

//	if (DEBUG) xer_fprint(stdout, &asn_DEF_MmsPdu, mmsPdu);

	//TODO solve deallocation problem
	freeInitiateResponsePdu(mmsPdu->choice.initiateResponsePdu);
	free(mmsPdu);

	//asn_DEF_MmsPdu.free_struct(&asn_DEF_MmsPdu, mmsPdu, 0);

	return rval.encoded;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static InitiateResponsePdu_t
  createInitiateResponsePdu(MmsServerConnection* self) {
/*----------------------------------------------------------------------------*/
	InitiateResponsePdu_t response;

	response.localDetailCalled = calloc(1, sizeof(Integer32_t));
	*(response.localDetailCalled) = self->maxPduSize;

	response.negotiatedMaxServOutstandingCalled =
			self->maxServOutstandingCalled;

	response.negotiatedMaxServOutstandingCalling =
			self->maxServOutstandingCalling;

	response.negotiatedDataStructureNestingLevel = calloc(1, sizeof(Integer8_t));
	*(response.negotiatedDataStructureNestingLevel) =
			self->dataStructureNestingLevel;

	response.mmsInitResponseDetail.negotiatedVersionNumber = 1;

	response.mmsInitResponseDetail.negotiatedParameterCBB.size = 2;
	response.mmsInitResponseDetail.negotiatedParameterCBB.bits_unused = 5;
	response.mmsInitResponseDetail.negotiatedParameterCBB.buf = calloc(2, 1);
	response.mmsInitResponseDetail.negotiatedParameterCBB.buf[0] = 0xf1;	//CHANGE _V_0_3_2_
	//TODO add CBB value

	response.mmsInitResponseDetail.servicesSupportedCalled.size = 11;
	response.mmsInitResponseDetail.servicesSupportedCalled.bits_unused = 3;
	response.mmsInitResponseDetail.servicesSupportedCalled.buf = calloc(11, sizeof(uint8_t));
	memcpy(response.mmsInitResponseDetail.servicesSupportedCalled.buf, servicesSupported, 11);

	return response;
}

static int
handleConfirmedRequestPdu( MmsServerConnection* self,ConfirmedRequestPdu_t*
	request, ByteBuffer* response) {
	long invokeIdLong;

	asn_INTEGER2long((INTEGER_t*) &(request->invokeID), &invokeIdLong);

	int32_t invokeId = (int32_t) invokeIdLong;

	//LWIP_DEBUGF(ISO9506_DEBUG, ("-----ISO9506: invokeId=%01d\r\n", invokeId));
	//LWIP_DEBUGF(ISO9506_DEBUG, ("-----ISO9506: present=%01d\r\n", request->
	//	confirmedServiceRequest.present));
	
	if (invokeId == 0) {
		asm("nop");
	}
	if (invokeId == 1) {
		asm("nop");
	}
	if (invokeId == 2) {
		asm("nop");
	}
	if (invokeId == 3) {
		asm("nop");
	}
	if (invokeId == 4) {
		asm("nop");
	}
	if (invokeId == 5) {
		asm("nop");
	}
	if (invokeId == 6) {
		asm("nop");
	}
	if (invokeId == 7) {
		asm("nop");
	}
	if (invokeId == 8) {
		asm("nop");
	}
	if (invokeId == 9) {
		asm("nop");
	}
	if (invokeId == 10) {
		asm("nop");
	}
	if (invokeId == 11) {
		asm("nop");
	}
	if (invokeId == 12) {
		asm("nop");
	}
	if (invokeId == 13) {
		asm("nop");
	}
	if (invokeId == 14) {
		asm("nop");
	}
	
	// надо принять:			LLN01$ST
	
	// надо пропустить: 	LLN01$GO
	//										LPHD1$ST
	//										LPHD1$DC
	//										GGIO0$ST
	//										GGIO0$MX
	
	//  LLN010GO0
	// 0xe268 -> 0xe250 - выделение variableId
	
	

	switch(request->confirmedServiceRequest.present) {
	case ConfirmedServiceRequest_PR_getNameList:
		mmsServer_handleGetNameListRequest(self, &(request->confirmedServiceRequest.
			choice.getNameList),invokeId, (ByteBufferPtr)response);
		break;
	case ConfirmedServiceRequest_PR_read:
	// временно
		mmsServer_handleReadRequest(self, &(request->confirmedServiceRequest.choice.read),
			invokeId, (ByteBufferPtr)response);
		break;
	case ConfirmedServiceRequest_PR_write:
	// временно
		mmsServer_handleWriteRequest(self, &(request->confirmedServiceRequest.choice.write),
			invokeId, (ByteBufferPtr)response);
		break;
	case ConfirmedServiceRequest_PR_getVariableAccessAttributes:
	// временно
		mmsServer_handleGetVariableAccessAttributesRequest(self, &(request->
			confirmedServiceRequest.choice.getVariableAccessAttributes), invokeId, 
			(ByteBufferPtr)response);
		break;
	case ConfirmedServiceRequest_PR_defineNamedVariableList:
		// не знаю для чего
		/*mmsServer_handleDefineNamedVariableListRequest(self,
				&(request->confirmedServiceRequest.choice.defineNamedVariableList),
				invokeId, response);*/
		asm("nop");
		break;
	case ConfirmedServiceRequest_PR_getNamedVariableListAttributes:
		// временно
		mmsServer_handleGetNamedVariableListAttributesRequest(self,
			&(request->confirmedServiceRequest.choice.getNamedVariableListAttributes),
			invokeId, (ByteBufferPtr)response);
		asm("nop");
		break;
	case ConfirmedServiceRequest_PR_deleteNamedVariableList:
		// не знаю для чего
		/*mmsServer_handleDeleteNamedVariableListRequest(self,
				&(request->confirmedServiceRequest.choice.deleteNamedVariableList),
				invokeId, response);*/
		asm("nop");
		break;
	default:
		//writeMmsRejectPdu(&invokeId, REJECT_UNRECOGNIZED_SERVICE, response);
		asm("nop");
		break;
	}

	return 0;
}

static void
	freeInitiateResponsePdu(InitiateResponsePdu_t response){
	free(response.localDetailCalled);
	free(response.negotiatedDataStructureNestingLevel);
	free(response.mmsInitResponseDetail.negotiatedParameterCBB.buf);
	free(response.mmsInitResponseDetail.servicesSupportedCalled.buf);
}

// вплетена в код - запрос со стороны read
MmsNamedVariableList
	MmsServerConnection_getNamedVariableList(MmsServerConnection* self, char* 
		variableListName){
	//TODO remove code duplication - similar to MmsDomain_getNamedVariableList !
	MmsNamedVariableList variableList = NULL;

	LinkedList element = LinkedList_getNext(self->namedVariableLists);

	while (element != NULL) {
		MmsNamedVariableList varList = (MmsNamedVariableList) element->data;

		if (strcmp(MmsNamedVariableList_getName(varList), variableListName) == 0) {
			variableList = varList;
			break;
		}

		element = LinkedList_getNext(element);
	}

	return variableList;
}

void
	MmsServerConnection_deleteNamedVariableList(MmsServerConnection* self, char* 
		listName) {
	mmsServer_deleteVariableList(self->namedVariableLists, listName);
}