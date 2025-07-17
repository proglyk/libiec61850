#include "libiec61850/IsoSession.h"
#include "libiec61850/IsoPresentation.h"
#include <string.h>
#include "libiec61850/SBuffer.h"

// Type definitions

struct sIsoSession {
  // Own needs
  u16_t      callingSessionSelector;
	u16_t      calledSessionSelector;
	u16_t      sessionRequirement;
	u8_t       protocolOptions;
	ByteBuffer userData;
  // Linkage with the upper layer
  IsoPresentationPtr isoPresent;
  SBufferPtr  sbuf;
};

static IsoSessionIndication IsoSession_parseMessage(IsoSessionPtr, ByteBuffer *);
static ByteBuffer *IsoSession_getUserData(IsoSessionPtr);

/**	----------------------------------------------------------------------------
	* @brief Iso Session layer constructor */
IsoSessionPtr
	IsoSession_Create(SBufferPtr sbuf) {
/*----------------------------------------------------------------------------*/
	// Self creating
  IsoSessionPtr self = calloc(1, sizeof(struct sIsoSession));
  if (!self) return NULL;
	self->sessionRequirement = 0x0002; /* default = duplex functional unit */
	self->callingSessionSelector = 0x0001;
	self->calledSessionSelector = 0x0001;
  // Top layers creating
  self->isoPresent = IsoPresentation_Create();
  if (!self->isoPresent) return NULL;
  self->sbuf = sbuf;
  
  return self;
}

/**	----------------------------------------------------------------------------
	* @brief Iso Session layer destructor */
void
  IsoSession_Delete(IsoSessionPtr self) {
/*----------------------------------------------------------------------------*/
	if (!self) return;
  if (self->isoPresent) IsoPresentation_Delete(self->isoPresent);
	free(self); self = NULL;
}

/**	----------------------------------------------------------------------------
	* @brief Iso Session layer action */
s32_t
  IsoSession_Do(IsoSessionPtr self, ByteBuffer *buf) {
/*----------------------------------------------------------------------------*/
  IsoSessionIndication sta;
  ByteBuffer* sessionUserData;
  // read body
  sta = IsoSession_parseMessage(self, buf);
  sessionUserData = IsoSession_getUserData(self);
  // см.протокол.
  if (sta == SESSION_CONNECT) {
    
  }
  // см.протокол.
  else if (sta == SESSION_DATA) {
//    if (self->state == COTP_CON_RUN) {
//      payload = CotpConnection_getPayload(self);
//      
//    }
  }
  // Exception
  else {
    goto exit;
  }
  
  
  return 0;
  
  exit:
  //self->state = SESSION_ERROR;
  return -1;
}

// Определения локальных (private) функций

/**	----------------------------------------------------------------------------
	* @brief Iso Session layer action */
static IsoSessionIndication
  IsoSession_parseMessage(IsoSessionPtr self, ByteBuffer* message) {
/*----------------------------------------------------------------------------*/
	u8_t* buffer = message->buffer;
	u8_t id;
	u8_t length;

	//session->userData = NULL;

	/* if (message->size > 1) {
		id = buffer[0];
		length = buffer[1];
	}
	else
		return SESSION_ERROR;

	switch (id) {
	case 13: // CONNECT(CN) SPDU

//		LWIP_DEBUGF(COTP_DEBUG, ("CONNECT(CN) SPDU\r\n"));
		if (length != (message->size - 2)){
//			LWIP_DEBUGF(COTP_DEBUG, ("SPDU Lenght - Bad\r\n"));
			return SESSION_ERROR;
		}
		if (parseSessionHeaderParameters(session, message, length) == SESSION_OK){
//			LWIP_DEBUGF(COTP_DEBUG, ("SPDU CONNECT - OK\r\n"));
			return SESSION_CONNECT;
		}
		else {
			//printf("iso_session.c: error parsing connect spdu\r\n");
			return SESSION_ERROR;
		}

		break;
	case 14: // ACCEPT SPDU
/*		if (length != (message->size - 2))
			return SESSION_ERROR;
		if (parseSessionHeaderParameters(session, message, length) == SESSION_OK)
			return SESSION_CONNECT;
		else {
			printf("iso_session.c: error parsing accept spdu\r\n");
			return SESSION_ERROR;
		}*/

/* 		break;
	case 1: // Give token / data SPDU
		if (message->size < 4)
			return SESSION_ERROR;
		if ((length == 0) && (buffer[2] == 1) && (buffer[3] == 0)) {
			//ByteBuffer* userData = calloc(1, sizeof(ByteBuffer));
			ByteBuffer_wrap(&session->userData, message->buffer + 4, message->size - 4, message->maxSize - 4);

			//session->userData = userData;
			return SESSION_DATA;
		}
		return SESSION_ERROR;

		break;
	} */

	return SESSION_ERROR;
}

/**	----------------------------------------------------------------------------
	* @brief Iso Session layer action */
static ByteBuffer*
  IsoSession_getUserData(IsoSessionPtr self) {
/*----------------------------------------------------------------------------*/
	return &session->userData;
}