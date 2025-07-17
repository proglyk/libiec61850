#include "libiec61850/IsoPresentation.h"
#include "libiec61850/AcseConnection.h"
#include "libiec61850/utils/byte_buffer.h"
//#include "libiec61850/iso8823/CPType.h"

// Type definitions

struct sIsoPresentation {
  // Own needs
	u32_t             callingPresentationSelector;
	u32_t             calledPresentationSelector;
	ContextDefinition context[2];
	u8_t              nextContextId;
	ByteBuffer        nextPayload;
  // Linkage with the upper layer
  AcseConnectionPtr acseConn;
  SBufferPtr        sbuf;
};

static IsoPresStatus parseConnectPdu(IsoPresentationPtr,  ByteBuffer *);

/**	----------------------------------------------------------------------------
	* @brief Iso Presentation layer constructor */
IsoPresentationPtr
	IsoPresentation_Create(SBufferPtr sbuf) {
/*----------------------------------------------------------------------------*/
	// Self creating
  IsoPresentationPtr self = calloc(1, sizeof(struct sIsoPresentation));
  if (!self) return NULL;
  // линкуем SBuffer
  self->sbuf = sbuf;
  // Top layers creating
  self->acseConn = AcseConnection_Create(self->sbuf);
  if (!self->acseConn) return NULL;
  
  return self;
}

/**	----------------------------------------------------------------------------
	* @brief Iso Presentation layer destructor */
void
  IsoPresentation_Delete(IsoPresentationPtr self) {
/*----------------------------------------------------------------------------*/
	if (!self) return;
  if (self->acseConn) AcseConnection_Delete(self->acseConn);
	free(self); self = NULL;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
void
	IsoPresentation_ThrowOverListener( IsoPresentationPtr self, MsgPassedHandlerPtr handler, void *param ) {
/*----------------------------------------------------------------------------*/
  AcseConnection_InstallListener(self->acseConn, handler, param);
}

/**	----------------------------------------------------------------------------
	* @brief Iso Presentation layer destructor */
s32_t
  IsoPresentation_parseConnect(IsoPresentationPtr self, ByteBuffer *buffer) {
/*----------------------------------------------------------------------------*/
	// ???
  IsoPresStatus sta = parseConnectPdu(self, buffer);
  if (sta != PRESENTATION_OK) return -1;
  //
  // TODO AcseConnection_parseMessage(self->acseConn, self->nextPayload);
  return 0;
}

// Определения локальных (private) функций

/**	----------------------------------------------------------------------------
	* @brief ??? */
static IsoPresStatus
  parseConnectPdu(IsoPresentationPtr self, ByteBuffer *buffer) {
/*----------------------------------------------------------------------------*/
	/* CPType_t* cptype = 0;
	asn_dec_rval_t rval;
  int pdvListEntries;
  int decodedOctets;
	int dataListEntries;
	int userDataSize;

  rval = ber_decode(NULL, &asn_DEF_CPType, (void**)&cptype,
    ByteBuffer_getBuffer(buffer), ByteBuffer_getSize(buffer));
  if (rval.code != RC_OK){
    if (ISO8823_DEBUG) printf("iso_presentation.c: broken CPType encoding at "
      "byte %ld\r\n",	(long) rval.consumed);
		goto error;
  }

	decodedOctets = rval.consumed;
	if (cptype->modeSelector.modeValue != (long)modeValue_normalmode) {
		if (ISO8823_DEBUG) printf("iso_presentation.c: only normal mode"
      "supported!");
		goto error;
	}
//  if (ISO8823_DEBUG) printf("modeSelector:= %02x\r\n",
//  	cptype->modeSelector.modeValue);

	pdvListEntries =
    cptype->normalModeParameters->contextDefinitionList->list.count;
	if (pdvListEntries != 2) {
		if (ISO8823_DEBUG) printf("iso_presentation: 2 pdv list items required"
      " found: %i\r\n", pdvListEntries);
		goto error;
	}
//  if (ISO8823_DEBUG) printf("pdvListEntries:= %02d\r\n", pdvListEntries);


	int i;
	for (i = 0; i < pdvListEntries; i++) {
		ContextListSeq_t* member =
				cptype->normalModeParameters->contextDefinitionList->list.array[i];

		if (isoPres_setContextDefinition(self, i, member) == PRESENTATION_ERROR)
			goto error;
	}

	if (cptype->normalModeParameters->userData->present ==
    Userdata_PR_fullyencodeddata){
		dataListEntries = cptype->normalModeParameters->userData->choice.
      fullyencodeddata.list.count;
		PDVlist_t* pdvListEntry = cptype->normalModeParameters->userData->choice.
      fullyencodeddata.list.array[0];
		userDataSize = pdvListEntry->presentationdatavalues.choice.singleASN1type.
      size;
		uint8_t* userData = pdvListEntry->presentationdatavalues.choice.
      singleASN1type.buf;
		self->nextContextId = (uint8_t)pdvListEntry->presentationcontextidentifier;

		ByteBuffer_wrap(&(self->nextPayload),buffer->buffer + (decodedOctets -
      userDataSize), userDataSize, userDataSize);
	} else {
		if (ISO8823_DEBUG) printf("iso_presentation.c: Unsupported user data: "
      "simply encoded data\r\n");
		goto error;
	}

//	if (ISO8823_DEBUG) printf("dataListEntries:= %02d\r\n",
//    dataListEntries);
//	if (ISO8823_DEBUG) printf("nextContextId:= %02d\r\n",
//    self->nextContextId);

  asn_DEF_CPType.free_struct(&asn_DEF_CPType, cptype, 0);
  return PRESENTATION_OK;
  error:
  asn_DEF_CPType.free_struct(&asn_DEF_CPType, cptype, 0); */
  return PRESENTATION_ERROR;
}