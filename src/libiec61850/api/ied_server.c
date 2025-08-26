//#include "HAL/stm32f4xx.h"
#include "libiec61850/api/ied_server_api.h"
#include "libiec61850/api/mms_mapping/mms_mapping.h"
#include "libiec61850/mms/mms_value.h"
#include "libiec61850/mms/mms_value_cache.h"
//#include <stdlib.h>
//#include "control.h"
#include "stack_config.h"
#include <string.h>
#include "libiec61850/api/mms_mapping/control.h"
//#include "heap_user.h"

extern ControlObject*
MmsMapping_getControlObject(MmsMapping* self, MmsDomain* domain, char* lnName, char* coName);

extern void
MmsMapping_addControlObject(MmsMapping* self, ControlObject* controlObject);


/*******************************************************************************
	Locally viewed (static) routines prototypes
*******************************************************************************/

static void
	createMmsServerCache(IedServer self);
	
static void
	installDefaultValuesForDataAttribute(IedServer self, DataAttribute* dataAttribute,
		char* objectReference, int position);
		
static void
	installDefaultValuesForDataObject(IedServer self, DataObject* dataObject,
		char* objectReference, int position);
		
static void
	updateDataSetsWithCachedValues(IedServer self);
	
static void
createControlObjects(IedServer self, MmsDomain* domain, char* lnName, MmsTypeSpecification* typeSpec);

static void
installDefaultValuesInCache(IedServer self);

/*******************************************************************************
	Locally viewed variables
*******************************************************************************/


static MmsValue* mms;
static Map cach;
//static Data_t* ldataElement = NULL;


/*******************************************************************************
	Globally viewed routines definitions
*******************************************************************************/

__weak void
ied_LedToggle(void) {
	
}

void
IedServer_lockDataModel(IedServer self)
{
	MmsServer_lockModel(self->mmsServer);
}

void
IedServer_unlockDataModel(IedServer self)
{
	MmsServer_unlockModel(self->mmsServer);
}


IedServer
	IedServer_create(IedModel* iedModel){
		
	IedServer self = calloc(1, sizeof(struct sIedServer));

	self->model = iedModel;

	self->mmsMapping = MmsMapping_create(iedModel);

	self->mmsDevice = MmsMapping_getMmsDeviceModel(self->mmsMapping);

	//self->isoServer = IsoServer_create();

	self->mmsServer = MmsServer_create(self->mmsDevice);
	
	MmsMapping_setMmsServer(self->mmsMapping, self->mmsServer);

	MmsMapping_installHandlers(self->mmsMapping);

	createMmsServerCache(self);

	iedModel->initializer();

	installDefaultValuesInCache(self);

	updateDataSetsWithCachedValues(self);

	return self;
}


void
	IedServer_searchCachedValues(IedServer self) {
	
	char * domain = "UPG";
	char * node = "CurData_GGIO1";
	char * cf = "MX";
	char * var = "UPGId";
	char * varat = "mag";
	char * varsubat = "f";
	
	MmsValueCache cache = Map_getEntry(self->mmsServer->valueCaches, self->mmsDevice->
		domains[0]);
	
	mms = MmsValueCache_lookupValue(cache, node);
	
//	ldataElement = mmsMsg_createBasicDataElement(mms);
		
  asm("nop");
}


void
	IedServer_printAllCachedValues(IedServer self) {
	
	MmsTypeSpecification* spec = NULL;
	int i = 0;
	
	//LWIP_DEBUGF(ISO9506_DEBUG, ("printAllCachedValues: %s\r\n", ((MmsDomain*)self->mmsDevice->
	//		domains[0])->domainName));
	//LWIP_DEBUGF(ISO9506_DEBUG, ("printAllCachedValues: %d\r\n", ((MmsDomain*)self->mmsDevice->
	//		domains[0])->namedVariablesCount));
	
	MmsDomain* mmsDomain = (MmsDomain*)self->mmsDevice->domains[0];
	
	spec = (MmsTypeSpecification*)mmsDomain->namedVariables[0];
	//LWIP_DEBUGF(ISO9506_DEBUG, ("printAllCachedValues: %s\r\n", spec->name));
	
	while (spec) {
		switch (spec->type) {
		case (MMS_STRUCTURE):
			spec = (MmsTypeSpecification*)spec->typeSpec.structure.elements[0];
			//LWIP_DEBUGF(ISO9506_DEBUG, ("printAllCachedValues: %s\r\n", spec->name));
			break;
		case (MMS_BIT_STRING):
			//LWIP_DEBUGF(ISO9506_DEBUG, ("printAllCachedValues: MMS_BIT_STRING=%�\r\n",
			//	(int)spec));
			spec = 0;
			break;
		default:
			//LWIP_DEBUGF(ISO9506_DEBUG, ("printAllCachedValues: error"));
			break;
		}
	}
	
	cach = self->mmsServer->valueCaches;
	
	asm("nop");
	
	//self->mmsServer->valueCaches->entries
	
	
	//(MmsTypeSpecification*)(((MmsDomain*)self->mmsDevice->domains[0])->namedVariables[0]))->type
	
	//self->
	//self->mmsServer->valueCaches->entries->next
	
//	MmsValueCache cache = Map_getEntry(self->mmsServer->valueCaches, self->mmsDevice->domains[0]);
	
//	mms = MmsValueCache_lookupValue(cache, "UPGId");
	
	asm("nop");
}


void
	IedServer_destroy(IedServer self) {
	
	// destroy пока не следует вызывать
	MmsServer_destroy(self->mmsServer);
	//IsoServer_destroy(self->isoServer); // ПОКА закоменчиваю
	MmsMapping_destroy(self->mmsMapping);
	free(self);
}


void
	IedServer_start(IedServer self, int tcpPort){
	
	//MmsMapping_startEventWorkerThread(self->mmsMapping);
	MmsServer_startListening(self->mmsServer, tcpPort);
}


void
IedServer_stop(IedServer self){
	MmsServer_stopListening(self->mmsServer);
}


MmsDomain*
IedServer_getDomain(IedServer self, char* logicalDeviceName){
	return MmsDevice_getDomain(self->mmsDevice, logicalDeviceName);
}


MmsValue*
IedServer_getValue(IedServer self, MmsDomain* domain, char* mmsItemId){
	return MmsServer_getValueFromCache(self->mmsServer, domain, mmsItemId);
}


MmsServer
IedServer_getMmsServer(IedServer self){
	return self->mmsServer;
}

//IsoServer
//IedServer_getIsoServer(IedServer self){
//	return self->isoServer;
//}


/**	----------------------------------------------------------------------------
	* @brief Обновление значения атрибута CHANGE _V_0_3_3_MYSELF
	* @param dataset: Номер текущего датасета посылки.
	* @param cargpage: Номер текущей страницы каретки.
	* @param ptr: Указатель на структуру со значениями из посылки.
	* @retval none: Нет */
bool
IedServer_updateAttributeValue(IedServer self, DataAttribute* node, MmsValue* 
	value) {
/*----------------------------------------------------------------------------*/
	DataAttribute* dataAttribute = (DataAttribute*) node;
	bool result = false;
	
	// Если не DataAttributeModelType, то выйти
	if (node->modelType != DataAttributeModelType)
		return false;
	
	if (MmsValue_isEqual(dataAttribute->mmsValue, value)) {
		// если новое значение совпадает со старым, то ничего не делаем
		// только извещаем RP
		MmsMapping_triggerReportObservers(self->mmsMapping, dataAttribute->mmsValue,
			REPORT_CONTROL_VALUE_UPDATE);
		result = false;
	} else {
		// если новое значение отличается от старого, то обновляем его
		MmsValue_update(dataAttribute->mmsValue, value);
		// извещаем RP
		//MmsMapping_triggerReportObservers(self->mmsMapping, dataAttribute->mmsValue,
		//  REPORT_CONTROL_VALUE_CHANGED);
		// TODO
		if (CONFIG_INCLUDE_GOOSE_SUPPORT==1)
			MmsMapping_triggerGooseObservers(self->mmsMapping, dataAttribute->mmsValue);
		result = true;
	}
	
	return result;
}


//CHANGE _VER_0_3_3_
void
	IedServer_updateAttributes(IedServer self, DataAttribute* attr_value, 
		DataAttribute* attr_time, MmsValue* value, MmsValue* time) {
	
	if (attr_value->modelType == DataAttributeModelType) {
		if (MmsValue_isEqual(attr_value->mmsValue, value)) {
		    MmsMapping_triggerReportObservers(self->mmsMapping, attr_value->mmsValue,
		            REPORT_CONTROL_VALUE_UPDATE);
		} else {
			MmsValue_update(attr_value->mmsValue, value);
			if (time->type==MMS_UTC_TIME)
				memcpy(attr_time->mmsValue->value.utcTime, time->value.utcTime, 8);
			//MmsValue_update(attr_time->mmsValue, time);
		    //MmsMapping_triggerReportObservers(self->mmsMapping, attr_value->mmsValue, REPORT_CONTROL_VALUE_CHANGED);
			if (CONFIG_INCLUDE_GOOSE_SUPPORT==1)
		  	MmsMapping_triggerGooseObservers(self->mmsMapping, attr_value->mmsValue);
		}
	}
}


//CHANGE _VER_0_3_3_
void
IedServer_attributeQualityChanged(IedServer self, ModelNode* node)
{
    if (node->modelType == DataAttributeModelType) {
        DataAttribute* dataAttribute = (DataAttribute*) node;

        MmsMapping_triggerReportObservers(self->mmsMapping, dataAttribute->mmsValue,
                   REPORT_CONTROL_QUALITY_CHANGED);
    }
}


//CHANGE _VER_0_3_3_
void
IedServer_enableGoosePublishing(IedServer self)
{
    MmsMapping_enableGoosePublishing(self->mmsMapping);
}


//CHANGE _VER_0_3_3_
void
	IedServer_observeDataAttribute(IedServer self, DataAttribute* dataAttribute,
		AttributeChangedHandler handler) {
			
	MmsMapping_addObservedAttribute(self->mmsMapping, dataAttribute, (void*)handler);
}


void
IedServer_setControlHandler(
        IedServer self,
        DataObject* node,
        ControlHandler listener,
        void* parameter)
{
		//ИСПРАВИЛ
		//	
    char objectReference[129];
		char* separator;

    ModelNode_getObjectReference((ModelNode*)node, objectReference);

    separator = strchr(objectReference, '/');

    *separator = 0;

    MmsDomain* domain = MmsDevice_getDomain(self->mmsDevice, objectReference);

    char* lnName = separator + 1;

    separator = strchr(lnName, '.');

    *separator = 0;

    char* objectName = separator + 1;

    separator = strchr(objectName, '.');

    if (separator != NULL)
        *separator = 0;

    ControlObject* controlObject = MmsMapping_getControlObject(self->mmsMapping, domain,
            lnName, objectName);

    if (controlObject != NULL)
        ControlObject_installListener(controlObject, listener, parameter);
}


/*******************************************************************************
	Locally viewed (static) routines definitions
*******************************************************************************/

static void
createControlObjects(IedServer self, MmsDomain* domain, char* lnName, MmsTypeSpecification* typeSpec)
{
    MmsMapping* mapping = self->mmsMapping;

    if (typeSpec->type == MMS_STRUCTURE) {
        int coCount = typeSpec->typeSpec.structure.elementCount;
        int i;
        for (i = 0; i < coCount; i++) {
            MmsTypeSpecification* coSpec = typeSpec->typeSpec.structure.elements[i];

            int coElementCount = coSpec->typeSpec.structure.elementCount;

            MmsValue* operVal = NULL;
            MmsValue* sboVal = NULL;
            MmsValue* sbowVal = NULL;
            MmsValue* cancelVal = NULL;

            ControlObject* controlObject = ControlObject_create(self->mmsServer, domain, lnName, coSpec->name);

            MmsValue* structure = MmsValue_newDefaultValue(coSpec);

            ControlObject_setMmsValue(controlObject, structure);

            int j;
            for (j = 0; j < coElementCount; j++) {
                MmsTypeSpecification*  coElementSpec = coSpec->typeSpec.structure.elements[j];

                if (strcmp(coElementSpec->name, "Oper") == 0) {
                    operVal = MmsValue_getStructElementByIndex(structure, j);

                    ControlObject_setOper(controlObject, operVal);
										
										//CHANGE _VER_0_3_3_
                    //MmsTypeSpecification* ctlValSpec =
                    //        coSpec->typeSpec.structure.elements[j]->typeSpec.structure.elements[0];

                    //MmsValue* ctlVal = MmsValue_newDefaultValue(ctlValSpec);
                }
                else if (strcmp(coElementSpec->name, "Cancel") == 0) {
                    cancelVal = MmsValue_getStructElementByIndex(structure, j);
                    ControlObject_setCancel(controlObject, cancelVal);
                }
                else if (strcmp(coElementSpec->name, "SBO") == 0) {
                    sboVal = MmsValue_getStructElementByIndex(structure, j);
                    ControlObject_setSBO(controlObject, sboVal);
                }
                else if (strcmp(coElementSpec->name, "SBOw") == 0) {
                    sbowVal = MmsValue_getStructElementByIndex(structure, j);
                    ControlObject_setSBOw(controlObject, sbowVal);
                }
                else {
                    //printf("createControlObjects: Unknown element in CO!\n");
                }
            }

            MmsMapping_addControlObject(mapping, controlObject);
        }
    }
}


//CHANGE _V_0_3_3_	
static void
createMmsServerCache(IedServer self)
{

	int domain = 0;

	for (domain = 0; domain < self->mmsDevice->domainCount; domain++) {

		/* Install all top level MMS named variables (=Logical nodes) in the MMS server cache */
		MmsDomain* logicalDevice = self->mmsDevice->domains[domain];

		int i;

		for (i = 0; i < logicalDevice->namedVariablesCount; i++) {
			char* lnName = logicalDevice->namedVariables[i]->name;

			//if (DEBUG) printf("Insert into cache %s - %s\n", logicalDevice->domainName, lnName);

			int fcCount = logicalDevice->namedVariables[i]->typeSpec.structure.elementCount;
			int j;

			for (j = 0; j < fcCount; j++) {
				MmsTypeSpecification* fcSpec = logicalDevice->namedVariables[i]->typeSpec.structure.elements[j];

				char* fcName = fcSpec->name;

				if (strcmp(fcName, "CO") == 0) {
				    int controlCount = fcSpec->typeSpec.structure.elementCount;

				    createControlObjects(self, logicalDevice, lnName, fcSpec);
				}
				else if ((strcmp(fcName, "BR") != 0) && (strcmp(fcName, "RP") != 0)
				        && (strcmp(fcName, "GO") != 0))
				{

					char* variableName = createString(3, lnName, "$", fcName);

					MmsValue* defaultValue = MmsValue_newDefaultValue(fcSpec);

					//if (DEBUG) printf("Insert into cache %s - %s\n", logicalDevice->domainName, variableName);
					MmsServer_insertIntoCache(self->mmsServer, logicalDevice, variableName, defaultValue);

					free(variableName);
				}
			}
		}

	}
}


static void
	installDefaultValuesForDataAttribute(IedServer self, DataAttribute* dataAttribute,
		char* objectReference, int position) {
	
	//sprintf(objectReference + position, ".%s", dataAttribute->name);

	char mmsVariableName[255]; //TODO check for optimal size

	MmsValue* value = dataAttribute->mmsValue;

	MmsMapping_createMmsVariableNameFromObjectReference(objectReference, 
		dataAttribute->fc, mmsVariableName);

	char domainName[100]; //TODO check for optimal size

	MmsMapping_getMmsDomainFromObjectReference(objectReference, domainName);

	MmsDomain* domain = MmsDevice_getDomain(self->mmsDevice, domainName);

	if (domain == NULL) {
		//printf("Error domain (%s) not found!\n", domainName);
		return;
	}

	MmsValue* cacheValue = MmsServer_getValueFromCache(self->mmsServer, domain, mmsVariableName);

	dataAttribute->mmsValue = cacheValue;

	if (value != NULL) {
		MmsValue_update(cacheValue, value);
		MmsValue_delete(value);
	}

	int childPosition = strlen(objectReference);
	DataAttribute* subDataAttribute = (DataAttribute*)dataAttribute->firstChild;

	while (subDataAttribute != NULL) {
		installDefaultValuesForDataAttribute(self, subDataAttribute, objectReference, childPosition);

		subDataAttribute = (DataAttribute*)subDataAttribute->sibling;
	}
}



static void
	installDefaultValuesForDataObject(IedServer self, DataObject* dataObject,
		char* objectReference, int position) {
	//sprintf(objectReference + position, ".%s", dataObject->name);

	ModelNode* childNode = dataObject->firstChild;

	int childPosition = strlen(objectReference);

	while (childNode != NULL) {
		if (childNode->modelType == DataObjectModelType) {
			installDefaultValuesForDataObject(self, (DataObject*)childNode, objectReference, childPosition);
		}
		else if (childNode->modelType == DataAttributeModelType) {
			installDefaultValuesForDataAttribute(self, (DataAttribute*)childNode, objectReference, childPosition);
		}

		childNode = childNode->sibling;
	}
}


static void
installDefaultValuesInCache(IedServer self)
{
	IedModel* model = self->model;

	char objectReference[255]; // TODO check for optimal buffer size;

	LogicalDevice* logicalDevice = model->firstChild;

	while (logicalDevice != NULL) {
		//sprintf(objectReference, "%s", logicalDevice->name);

		LogicalNode* logicalNode = logicalDevice->firstChild;

		char* nodeReference = objectReference + strlen(objectReference);

		while (logicalNode != NULL) {
			//sprintf(nodeReference, "/%s", logicalNode->name);

			DataObject* dataObject = (DataObject*)logicalNode->firstChild;

			int refPosition = strlen(objectReference);

			while (dataObject != NULL) {
				installDefaultValuesForDataObject(self, dataObject, objectReference, refPosition);

				dataObject = (DataObject*)dataObject->sibling;
			}

			logicalNode = (LogicalNode*)logicalNode->sibling;
		}

		logicalDevice = logicalDevice->sibling;
	}
}


static void
	updateDataSetsWithCachedValues(IedServer self) {
	
	DataSetPtr* dataSets = self->model->dataSets;

	int i = 0;
	while (((DataSetPtr)*(dataSets+i)) != NULL) {

		int fcdaCount = ((DataSetPtr)*(dataSets+i))->elementCount;

		int j = 0;

		for (j = 0; j < fcdaCount; j++) {

			MmsDomain* domain = MmsDevice_getDomain(self->mmsDevice, ((DataSetPtr)*(dataSets+i))->fcda[j]->logicalDeviceName);

			MmsValue* value = MmsServer_getValueFromCache(self->mmsServer, domain, ((DataSetPtr)*(dataSets+i))->fcda[j]->variableName);

			if (value == NULL) {
				//printf("error cannot get value from cache!\n");
      }
			else
				((DataSetPtr)*(dataSets+i))->fcda[j]->value = value;

		}

		i++;
	}
}




