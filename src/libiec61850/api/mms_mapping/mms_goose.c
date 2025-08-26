#include "stack_config.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "libiec61850/api/mms_mapping/mms_mapping.h"
#include "libiec61850/utils/linked_list.h"
#include "libiec61850/utils/array_list.h"

//#include "thread.h"

#include "libiec61850/api/mms_mapping/reporting.h"
#include "libiec61850/api/mms_mapping/mms_mapping_internal.h"

#include "libiec61850/api/mms_mapping/mms_goose.h"
#include "libiec61850/goose/goose.h"
#include "libiec61850/goose/goose_publisher.h"

#include "hal.h"
//#include "heap_user.h"

struct sMmsGooseControlBlock {
    char* name;
    bool goEna;

    char* dstAddress;

    MmsDomain* domain;
    LogicalNode* logicalNode;
    GooseControlBlock gcb;
    MmsTypeSpecification* mmsType;
    MmsValue* mmsValue;
    GoosePublisher* publisher;
    DataSetPtr dataSet;
    LinkedList dataSetValues;
    uint64_t nextPublishTime;
    Semaphore* publisherMutex;
};

MmsGooseControlBlock
MmsGooseControlBlock_create()
{
    MmsGooseControlBlock self = calloc(1, sizeof(struct sMmsGooseControlBlock));

    self->publisherMutex = Semaphore_create(1);

    return self;
}

void
MmsGooseControlBlock_destroy(MmsGooseControlBlock self)
{
    Semaphore_destroy(self->publisherMutex);

    if (self->publisher != NULL)
        GoosePublisher_destroy(self->publisher);

    if (self->dataSetValues != NULL)
        LinkedList_destroyStatic(self->dataSetValues);

    GooseControlBlock_destroy(self->gcb);

    MmsValue_delete(self->mmsValue);

    free(self);
}

MmsDomain*
MmsGooseControlBlock_getDomain(MmsGooseControlBlock self)
{
    return self->domain;
}

DataSetPtr
MmsGooseControlBlock_getDataSet(MmsGooseControlBlock self)
{
    return self->dataSet;
}

char*
MmsGooseControlBlock_getLogicalNodeName(MmsGooseControlBlock self)
{
    return self->logicalNode->name;
}

char*
MmsGooseControlBlock_getName(MmsGooseControlBlock self)
{
    return self->name;
}

MmsValue*
MmsGooseControlBlock_getMmsValues(MmsGooseControlBlock self)
{
    return self->mmsValue;
}

bool
MmsGooseControlBlock_isEnabled(MmsGooseControlBlock self)
{
    return self->goEna;
}

void
MmsGooseControlBlock_enable(MmsGooseControlBlock self)
{
    Semaphore_wait(self->publisherMutex);

    if (!MmsGooseControlBlock_isEnabled(self)) {
        MmsValue* goEna = MmsValue_getStructElementByIndex(self->mmsValue, 0);

        MmsValue_setBoolean(goEna, true);

        self->gcb->sqNum = 0;
        self->gcb->stNum = 1;

        if (self->publisher == NULL) {
            self->publisher = GoosePublisher_create(NULL, NULL);
        }

        //prepare data set values
        self->dataSetValues = LinkedList_create();

        DataSetPtr dataSet = self->dataSet;

        int i = 0;
        for (i = 0; i < dataSet->elementCount; i++) {
            LinkedList_add(self->dataSetValues, dataSet->fcda[i]->value);
        }

        self->goEna = true;
    }

    Semaphore_post(self->publisherMutex);
}

void
MmsGooseControlBlock_disable(MmsGooseControlBlock self)
{
    if (MmsGooseControlBlock_isEnabled(self)) {
        MmsValue* goEna = MmsValue_getStructElementByIndex(self->mmsValue, 0);

        MmsValue_setBoolean(goEna, false);

        self->goEna = false;

        Semaphore_wait(self->publisherMutex);

        if (self->publisher != NULL) {
            GoosePublisher_destroy(self->publisher);
            self->publisher = NULL;
            LinkedList_destroyStatic(self->dataSetValues);
            self->dataSetValues = NULL;
        }

        Semaphore_post(self->publisherMutex);
    }
}


void
	MmsGooseControlBlock_checkAndPublish(MmsGooseControlBlock self) {
	
	uint32_t timestamp[2] = {0,0};
	//GET_SYSTEM_TIME((timestamp+0),(timestamp+1)); TODO
	
	if (timestamp[0] >= self->nextPublishTime) {

		Semaphore_wait(self->publisherMutex);

		MmsValue_setUtcTimeMs(self->gcb->timestamp, timestamp);

		self->gcb->sqNum++;

		GoosePublisher_publish(self->publisher, self->gcb, self->dataSetValues);

		self->nextPublishTime = timestamp[0] + CONFIG_GOOSE_STABLE_STATE_TRANSMISSION_INTERVAL;

		Semaphore_post(self->publisherMutex);
	}
}

void
MmsGooseControlBlock_observedObjectChanged(MmsGooseControlBlock self)
{
    //uint64_t currentTime = Hal_getTimeInMs();
		//uint64_t currentTime = (uint64_t)HAL_GetTick();
		uint32_t timestamp[2] = {0,0};
		//GET_SYSTEM_TIME((timestamp+0),(timestamp+1)); TODO

    Semaphore_wait(self->publisherMutex);

    self->gcb->sqNum = 0;
    self->gcb->stNum++;

    self->nextPublishTime = timestamp[0] + CONFIG_GOOSE_STABLE_STATE_TRANSMISSION_INTERVAL;

    MmsValue_setUtcTimeMs(self->gcb->timestamp, timestamp);
		

    GoosePublisher_publish(self->publisher, self->gcb, self->dataSetValues);

    Semaphore_post(self->publisherMutex);
}


MmsValue*
MmsGooseControlBlock_getGCBValue(MmsGooseControlBlock self, char* elementName)
{
	MmsValue* temp = NULL;
	
    if (strcmp(elementName, "GoEna") == 0)
			return MmsValue_getStructElementByIndex(self->mmsValue, 0);
    else if (strcmp(elementName, "GoID") == 0)
			return MmsValue_getStructElementByIndex(self->mmsValue, 1);
    else if (strcmp(elementName, "DatSet") == 0)
			return MmsValue_getStructElementByIndex(self->mmsValue, 2);
    else if (strcmp(elementName, "ConfRev") == 0)
			return MmsValue_getStructElementByIndex(self->mmsValue, 3);
    else if (strcmp(elementName, "NdsCom") == 0)
			return MmsValue_getStructElementByIndex(self->mmsValue, 4);
    else if (strcmp(elementName, "DstAddress") == 0)
			return MmsValue_getStructElementByIndex(self->mmsValue, 5);
		else if (!strcmp(elementName, "DstAddress$Addr")) 
			return MmsValue_getStructElementByIndex(MmsValue_getStructElementByIndex(self->mmsValue, 5), 0);
		else if (!strcmp(elementName, "DstAddress$PRIORITY"))
			return MmsValue_getStructElementByIndex(MmsValue_getStructElementByIndex(self->mmsValue, 5), 1);
		else if (!strcmp(elementName, "DstAddress$VID"))
			return MmsValue_getStructElementByIndex(MmsValue_getStructElementByIndex(self->mmsValue, 5), 2);
		else if (!strcmp(elementName, "DstAddress$APPID"))
			return MmsValue_getStructElementByIndex(MmsValue_getStructElementByIndex(self->mmsValue, 5), 3);
    else if (strcmp(elementName, "MinTime") == 0)
			return MmsValue_getStructElementByIndex(self->mmsValue, 6);
    else if (strcmp(elementName, "MaxTime") == 0)
			return MmsValue_getStructElementByIndex(self->mmsValue, 7);
    else if (strcmp(elementName, "FixedOffs") == 0)
			return MmsValue_getStructElementByIndex(self->mmsValue, 8);

    return NULL ;
}

static MmsTypeSpecification*
createMmsGooseControlBlock(char* gcbName)
{
    MmsTypeSpecification* gcb = calloc(1, sizeof(MmsTypeSpecification));
    gcb->name = copyString(gcbName);
    gcb->type = MMS_STRUCTURE;
    gcb->typeSpec.structure.elementCount = 9;
    gcb->typeSpec.structure.elements = calloc(9, sizeof(MmsTypeSpecification*));

    MmsTypeSpecification* namedVariable;

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("GoEna");
    namedVariable->type = MMS_BOOLEAN;

    gcb->typeSpec.structure.elements[0] = namedVariable;

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("GoID");
    namedVariable->typeSpec.visibleString = -129;
    namedVariable->type = MMS_VISIBLE_STRING;

    gcb->typeSpec.structure.elements[1] = namedVariable;

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("DatSet");
    namedVariable->typeSpec.visibleString = -129;
    namedVariable->type = MMS_VISIBLE_STRING;

    gcb->typeSpec.structure.elements[2] = namedVariable;

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("ConfRev");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 32;

    gcb->typeSpec.structure.elements[3] = namedVariable;

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("NdsCom");
    namedVariable->type = MMS_BOOLEAN;

    gcb->typeSpec.structure.elements[4] = namedVariable;

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("DstAddress");
		namedVariable->type = MMS_STRUCTURE;
    namedVariable->typeSpec.structure.elementCount = 4;
    namedVariable->typeSpec.structure.elements = calloc(4,sizeof(MmsTypeSpecification*));
    MmsMapping_createPhyComAddrStructure(namedVariable); //ERROR

    gcb->typeSpec.structure.elements[5] = namedVariable;

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("MinTime");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 32;

    gcb->typeSpec.structure.elements[6] = namedVariable;

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("MaxTime");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 32;

    gcb->typeSpec.structure.elements[7] = namedVariable;

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("FixedOffs");
    namedVariable->type = MMS_BOOLEAN;
		namedVariable->typeSpec.boolean = 0;

    gcb->typeSpec.structure.elements[8] = namedVariable;

    return gcb;
}

static GSEControlBlockPtr
getGCBForLogicalNodeWithIndex(MmsMapping* self, LogicalNode* logicalNode, int index)
{
    int gseCount = 0;
		int i = 0;

    GSEControlBlockPtr* p_gseControlBlocks = self->model->gseCBs;
		GSEControlBlockPtr gseControlBlocks = (GSEControlBlockPtr)*(p_gseControlBlocks);
    

    while ((gseControlBlocks + i) != NULL ) {
        if ((gseControlBlocks + i)->parent == logicalNode) {
            if (gseCount == index)
                return (gseControlBlocks + i);

            gseCount++;
        }

        i++;
    }

    return NULL ;
}

static char*
createDataSetReference(char* domainName, char* lnName, char* dataSetName)
{
    char* dataSetReference;

    dataSetReference = createString(5, domainName, "/", lnName, "$", dataSetName);

    return dataSetReference;
}

MmsTypeSpecification*
GOOSE_createGOOSEControlBlocks(MmsMapping* self, MmsDomain* domain,
        LogicalNode* logicalNode, int gseCount)
{
    MmsTypeSpecification* namedVariable = calloc(1,
            sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("GO");
    namedVariable->type = MMS_STRUCTURE;

    namedVariable->typeSpec.structure.elementCount = gseCount;
    namedVariable->typeSpec.structure.elements = calloc(gseCount,
            sizeof(MmsTypeSpecification*));

    int currentGCB = 0;
		
		//LWIP_DEBUGF(ISO9506_DEBUG, ("GOOSE_createGOOSEControlBlocks\r\n"));

    while (currentGCB < gseCount) {
        GSEControlBlockPtr gooseControlBlock = getGCBForLogicalNodeWithIndex(
                self, logicalNode, currentGCB);

        MmsTypeSpecification* gseTypeSpec = createMmsGooseControlBlock(gooseControlBlock->name);

        MmsValue* gseValues = MmsValue_newStructure(gseTypeSpec);

        namedVariable->typeSpec.structure.elements[currentGCB] = gseTypeSpec;

        MmsGooseControlBlock mmsGCB = MmsGooseControlBlock_create();

        GooseControlBlock gcb = GooseControlBlock_create();

        gcb->timeAllowedToLive = CONFIG_GOOSE_STABLE_STATE_TRANSMISSION_INTERVAL * 3;

        gcb->goCBRef = createString(5, MmsDomain_getName(domain), "/", logicalNode->name,
                "$GO$", gooseControlBlock->name);

        if (gooseControlBlock->appId != NULL) {
            gcb->goID = copyString(gooseControlBlock->appId);
            MmsValue* goID = MmsValue_getStructElementByIndex(gseValues, 1);

            MmsValue_setVisibleString(goID, gcb->goID);
        }

        gcb->dataSetRef = createDataSetReference(MmsDomain_getName(domain),
                logicalNode->name, gooseControlBlock->dataSetName);

        MmsValue* dataSetRef = MmsValue_getStructElementByIndex(gseValues, 2);

        MmsValue_setVisibleString(dataSetRef, gcb->dataSetRef);

        /* Set communication parameters */
        uint8_t priority = CONFIG_GOOSE_DEFAULT_PRIORITY;
        uint8_t dstAddr[] = CONFIG_GOOSE_DEFAULT_DST_ADDRESS;
        uint16_t vid = CONFIG_GOOSE_DEFAULT_VLAN_ID;
        uint16_t appId = CONFIG_GOOSE_DEFAULT_APPID;

        MmsValue* dstAddress = MmsValue_getStructElementByIndex(gseValues, 5);
        MmsValue* addr = MmsValue_getStructElementByIndex(dstAddress, 0);
        MmsValue_setOctetString(addr, dstAddr, 6);

        MmsValue* prio = MmsValue_getStructElementByIndex(dstAddress, 1);
        MmsValue_setUint8(prio, priority);

        MmsValue* vlanId = MmsValue_getStructElementByIndex(dstAddress, 2);
        MmsValue_setUint16(vlanId, vid);

        MmsValue* appIdVal = MmsValue_getStructElementByIndex(dstAddress, 3);
        MmsValue_setUint16(appIdVal, appId);


        mmsGCB->dataSet = IedModel_lookupDataSet(self->model, gcb->dataSetRef);

        gcb->timestamp = MmsValue_newUtcTime(0);

        mmsGCB->mmsValue = gseValues;
        mmsGCB->mmsType = gseTypeSpec;
        mmsGCB->name = gooseControlBlock->name;

        mmsGCB->domain = domain;
        mmsGCB->logicalNode = logicalNode;
        mmsGCB->gcb = gcb;

        LinkedList_add(self->gseControls, mmsGCB);

        currentGCB++;
    }

    return namedVariable;
}

