#ifndef MMS_MAPPING_H_
#define MMS_MAPPING_H_

#include "libiec61850/api/model.h"
#include "libiec61850/mms/mms_server_conn.h"
#include "libiec61850/mms/mms_device.h"
//#include "libiec61850/api/mms_mapping/control.h"
//#include "libiec61850/api/mms_mapping/control.h"


typedef struct sMmsMapping MmsMapping;

typedef struct sAttributeObserver {
    DataAttribute* attribute;
    void (*handler) (DataAttribute* dataAttribute);
} AttributeObserver;

typedef enum {
    REPORT_CONTROL_NONE,
    REPORT_CONTROL_VALUE_UPDATE,
    REPORT_CONTROL_VALUE_CHANGED,
    REPORT_CONTROL_QUALITY_CHANGED
} ReportInclusionFlag;

MmsMapping*
MmsMapping_create(IedModel* model);

MmsDevice*
MmsMapping_getMmsDeviceModel(MmsMapping* mapping);

void
MmsMapping_setMmsServer(MmsMapping* self, MmsServer server);

void
MmsMapping_installHandlers(MmsMapping* self);

void
MmsMapping_destroy(MmsMapping* mapping);

void
MmsMapping_checkIfVariableIsObserved(MmsMapping* self, MmsValue* value);

char*
MmsMapping_getMmsDomainFromObjectReference(char* objectReference,char* buffer);

char*
MmsMapping_createMmsVariableNameFromObjectReference(char* objectReference, 
	FunctionalConstraint fc, char* buffer);

void
MmsMapping_addObservedAttribute(MmsMapping* self, DataAttribute* dataAttribute,
        void* handler);
				
void
MmsMapping_startEventWorkerThread(MmsMapping* self);

void
MmsMapping_triggerReportObservers(MmsMapping* self, MmsValue* value, ReportInclusionFlag flag);

void
MmsMapping_triggerGooseObservers(MmsMapping* self, MmsValue* value);

void
MmsMapping_enableGoosePublishing(MmsMapping* self);

MmsValue*
readAccessGooseControlBlock(MmsMapping* self, MmsDomain* domain, char* variableIdOrig);

bool
checkIfValueBelongsToModelNode(DataAttribute* dataAttribute, MmsValue* value);

void
GOOSE_processGooseEvents(MmsMapping* self);

// закоменчены, т.к. создавали перекрестную ссылку по object.h

//ControlObject*
//MmsMapping_getControlObject(MmsMapping* self, MmsDomain* domain, char* lnName, char* coName);

//void
//MmsMapping_addControlObject(MmsMapping* self, ControlObject* controlObject);

//MmsValue*
//readAccessControlObject(MmsMapping* self, MmsDomain* domain, char* variableIdOrig);

void /* Create PHYCOMADDR ACSI type instance */
MmsMapping_createPhyComAddrStructure(MmsTypeSpecification* namedVariable);

#endif /* MMS_MAPPING_H_ */
