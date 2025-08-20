#include "stack_config.h"
#include "libiec61850/mms/mms_server_conn.h"

static MmsIndication
	parseMmsPdu(MmsServerConnection* self, ByteBuffer* message, SBuffer*
		response);

/**	----------------------------------------------------------------------------
	* @brief Acse Connection layer constructor */
MmsServerConnection*
  MmsServerConnection_init( MmsServerConnection* connection, 
                            MmsServer server/* , IsoConnection isoCon */ ) {
/*----------------------------------------------------------------------------*/
	MmsServerConnection* self;

	if (connection == NULL)
		self = calloc(1, sizeof(MmsServerConnection));
	else
		self = connection;

	self->maxServOutstandingCalled = 0;
	self->maxServOutstandingCalling = 0;
	self->maxPduSize = MMS_MAXIMUM_PDU_SIZE;
	self->dataStructureNestingLevel = 0;
	self->server = server;
	//self->isoConnection = isoCon;
	//self->namedVariableLists = LinkedList_create();

	//IsoConnection_installListener(isoCon, messageReceived, (void*) self);

	return self;
}

void
	MmsServerConnection_destroy(MmsServerConnection* self){
	
	//LinkedList_destroyDeep(self->namedVariableLists, 
	//	(void (*)(void *))MmsNamedVariableList_destroy);
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

/**	----------------------------------------------------------------------------
	* @brief Acse Connection layer constructor */
static MmsIndication
  parseMmsPdu( MmsServerConnection* self, ByteBuffer* message, SBuffer* response) {
/*----------------------------------------------------------------------------*/
  return MMS_ERROR;
}