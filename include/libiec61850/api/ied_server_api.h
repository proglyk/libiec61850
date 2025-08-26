#ifndef __IED_SERVER_API_H__
#define __IED_SERVER_API_H__

#include "libiec61850/api/model.h"
#include "libiec61850/mms/mms_device.h"
#include "libiec61850/mms/mms_server.h"
#include "libiec61850/api/mms_mapping/mms_mapping.h"


typedef struct sIedServer {
	IedModel* model;
	MmsDevice* mmsDevice;
	MmsServer mmsServer;
	//IsoServer isoServer;
	MmsMapping* mmsMapping;
} * IedServer;

typedef bool (*ControlHandler) (void* parameter, MmsValue* ctlVal);

typedef void (*AttributeChangedHandler) (DataAttribute* dataAttribute);


IedServer
IedServer_create(IedModel* iedModel);

void
IedServer_destroy(IedServer self);

void
IedServer_start(IedServer self, int tcpPort);

void
IedServer_stop(IedServer self);

bool
IedServer_isRunning(IedServer self);

//MmsValue*
//IedServer_getAttributeValue(IedServer self, ModelNode* node);

bool
IedServer_updateAttributeValue(IedServer self, DataAttribute* node, MmsValue* value);

void
	IedServer_updateAttributes(IedServer self, DataAttribute* attr_value, 
		DataAttribute* attr_time, MmsValue* value, MmsValue* time);

void
IedServer_lockDataModel(IedServer self);

void
IedServer_unlockDataModel(IedServer self);

void
IedServer_setControlHandler(IedServer self, DataObject* node, ControlHandler handler, void* parameter);

//void
//IedServer_attributeQualityChanged(IedServer self, ModelNode* node);

void
IedServer_enableGoosePublishing(IedServer self);

void
IedServer_observeDataAttribute(IedServer self, DataAttribute* dataAttribute,
        AttributeChangedHandler handler);

#endif /*__IED_SERVER_API_H__*/