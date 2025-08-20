//#include "HAL/stm32f4xx.h"
#include "libiec61850/MmsServerConnectionInt.h"
#include <stdlib.h>

/* #include "server/iso_connection2.h"
#include <stdlib.h>
#include "server/byte_buffer.h"
#include "iso9506/MmsPdu.h"
#include "ied/mms/mms_server_common.h"
//#include "ied/mms/mms_service_named_variable_list.h"
#include "ied/mms/mms_service_read.h"
#include "ied/mms/mms_service_write.h"
#include "server/stack_config.h"
#include "ied/mms/mms_server_common.h"
#include "ied/mms/mms_service_named_variable_list.h" */


/* extern void mmsServer_handleGetNameListRequest(	MmsServerConnection* connection,
	GetNameListRequest_t* getNameList, int invokeId, ByteBuffer* response);

extern int
	mmsServer_handleGetVariableAccessAttributesRequest(MmsServerConnection* 
		connection,	GetVariableAccessAttributesRequest_t* request,		int invokeId,
		ByteBuffer* response);
	
//extern void	
//	mmsServer_handleGetNamedVariableListAttributesRequest(MmsServerConnection* 
//		connection, GetNamedVariableListAttributesRequest_t* request, int invokeId,
//		ByteBuffer* response);


static void
	messageReceived(void* parameter, ByteBuffer* message, SBuffer*	response);
static MmsIndication
	parseMmsPdu(MmsServerConnection* self, ByteBuffer* message, SBuffer*
		response);
static int
	handleInitiateRequestPdu(MmsServerConnection* self,	InitiateRequestPdu_t*
		request, ByteBuffer* response);
static void parseInitiateRequestPdu(MmsServerConnection* self,
	const InitiateRequestPdu_t* request);
static int createInitiateResponse(MmsServerConnection* self, ByteBuffer*
	writeBuffer);
static InitiateResponsePdu_t createInitiateResponsePdu(MmsServerConnection*
	self);
static void	freeInitiateResponsePdu(InitiateResponsePdu_t response);
static int handleConfirmedRequestPdu( MmsServerConnection* self,
	ConfirmedRequestPdu_t* request, ByteBuffer* response);


static uint8_t servicesSupported[] = {
		0x00
		| MMS_SERVICE_GET_NAME_LIST
		| MMS_SERVICE_READ
		| MMS_SERVICE_WRITE
		| MMS_SERVICE_GET_VARIABLE_ACCESS_ATTRIBUTES
		| MMS_SERVICE_GET_NAMED_VARIABLE_LIST_ATTRIBUTES
		,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00
};
 */

//CHANGE _V_0_3_0_
/**	----------------------------------------------------------------------------
	* @brief Constructor */
MmsServerConnection*
  MmsServerConnection_init(MmsServerConnection* connection, MmsServer server) {
/*----------------------------------------------------------------------------*/
	MmsServerConnection* self;

	if (connection == NULL)
		self = calloc(1, sizeof(MmsServerConnection));
	else
		self = connection;

/* 	self->maxServOutstandingCalled = 0;
	self->maxServOutstandingCalling = 0;
	self->maxPduSize = MMS_MAXIMUM_PDU_SIZE;
	self->dataStructureNestingLevel = 0;
	self->server = server;
	self->isoConnection = isoCon;
	self->namedVariableLists = LinkedList_create(); */

	//IsoConnection_installListener(isoCon, messageReceived, (void*) self);

	return self;
}

/**	----------------------------------------------------------------------------
	* @brief Constructor */
void
MmsServerConnection_destroy(MmsServerConnection* self) {
/*----------------------------------------------------------------------------*/
	//LinkedList_destroyDeep( self->namedVariableLists, 
  //                        (void (*)(void *))MmsNamedVariableList_destroy );
	free(self);
}
