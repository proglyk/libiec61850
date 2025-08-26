/*
 *  reporting.c
 *
 *  Copyright 2013 Michael Zillgith
 *
 *  This file is part of libIEC61850.
 *
 *  libIEC61850 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libIEC61850 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "libiec61850/api/mms_mapping/mms_mapping.h"
#include "libiec61850/utils/linked_list.h"
#include "libiec61850/utils/array_list.h"
#include "stack_config.h"
#include "hardware/thread.h"

#include "libiec61850/api/mms_mapping/reporting.h"
#include "libiec61850/api/mms_mapping/mms_mapping_internal.h"

#include "hal.h"
#include "libiec61850/api/mms_mapping/mms_information_report.h"
//#include "heap_user.h"
//#include "ethernetif/sntp.h"


/*******************************************************************************
	Locally viewed (static) routines prototypes
*******************************************************************************/

static void
updateTimeOfEntry(ReportControl* self, uint64_t currentTime);

static void
sendReport(ReportControl* self, bool isIntegrity, bool isGI);

static char*
createDataSetReference(ReportControlBlockPtr rcb, ReportControl* reportControl);

static MmsValue*
createOptFlds(ReportControlBlockPtr reportControlBlock);

static MmsValue*
createTrgOps(ReportControlBlockPtr reportControlBlock);

static MmsTypeSpecification*
	createUnbufferedReportControlBlock(ReportControlBlockPtr reportControlBlock,
		ReportControl* reportControl);
		
static MmsTypeSpecification*
	createBufferedReportControlBlock(ReportControlBlockPtr reportControlBlock,
		ReportControl* reportControl);
		
static ReportControlBlockPtr
	getRCBForLogicalNodeWithIndex(MmsMapping* self, LogicalNode* logicalNode,
		int index, bool buffered);

/*******************************************************************************
	Globally viewed routines definitions
*******************************************************************************/

ReportControl*
ReportControl_create(bool buffered)
{
    ReportControl* self = malloc(sizeof(ReportControl));
    self->name = NULL;
    self->domain = NULL;
    self->rcbValues = NULL;
    self->confRev = NULL;
    self->enabled = false;
    self->bufferd = buffered;
    self->gi = false;
    self->inclusionField = NULL;
    self->dataSet = NULL;
    self->clientConnection = NULL;
    self->intgPd = 0;
    self->sqNum = 0;
    self->nextIntgReportTime = 0;
    self->inclusionFlags = NULL;
    self->triggered = false;
    self->timeOfEntry = NULL;
    return self;
}


void
ReportControl_destroy(ReportControl* self)
{
    if (self->rcbValues != NULL )
        MmsValue_delete(self->rcbValues);

    if (self->inclusionFlags != NULL)
        free(self->inclusionFlags);

    if (self->inclusionField != NULL)
        MmsValue_delete(self->inclusionField);

    if (self->bufferd == false)
        MmsValue_delete(self->timeOfEntry);

    free(self->name);

    free(self);
}


MmsValue*
ReportControl_getRCBValue(ReportControl* rc, char* elementName)
{
    if (rc->bufferd) {
        if (strcmp(elementName, "RptID") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 0);
        else if (strcmp(elementName, "RptEna") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 1);
        else if (strcmp(elementName, "DatSet") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 2);
        else if (strcmp(elementName, "ConfRev") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 3);
        else if (strcmp(elementName, "OptFlds") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 4);
        else if (strcmp(elementName, "BufTm") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 5);
        else if (strcmp(elementName, "SqNum") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 6);
        else if (strcmp(elementName, "TrgOps") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 7);
        else if (strcmp(elementName, "IntgPd") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 8);
        else if (strcmp(elementName, "GI") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 9);
        else if (strcmp(elementName, "PurgeBuf") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 10);
        else if (strcmp(elementName, "EntryID") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 11);
        else if (strcmp(elementName, "TimeOfEntry") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 12);
        else if (strcmp(elementName, "ResvTms") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 13);
        else if (strcmp(elementName, "Owner") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 14);
    } else {
        if (strcmp(elementName, "RptID") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 0);
        else if (strcmp(elementName, "RptEna") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 1);
        else if (strcmp(elementName, "Resv") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 2);
        else if (strcmp(elementName, "DatSet") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 3);
        else if (strcmp(elementName, "ConfRev") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 4);
        else if (strcmp(elementName, "OptFlds") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 5);
        else if (strcmp(elementName, "BufTm") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 6);
        else if (strcmp(elementName, "SqNum") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 7);
        else if (strcmp(elementName, "TrgOps") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 8);
        else if (strcmp(elementName, "IntgPd") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 9);
        else if (strcmp(elementName, "GI") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 10);
        else if (strcmp(elementName, "Owner") == 0)
            return MmsValue_getStructElementByIndex(rc->rcbValues, 11);
    }

    return NULL ;
}


void
ReportControl_triggerGI(ReportControl* self)
{
    self->gi = true;
}


//CHANGE _VER_0_3_3_
void
ReportControl_valueUpdated(ReportControl* self, int dataSetEntryIndex, ReportInclusionFlag flag)
{
    self->inclusionFlags[dataSetEntryIndex] = flag;

    if (self->triggered == false) {
        uint64_t currentTime = Hal_getTimeInMs();

        MmsValue* timeOfEntry = self->timeOfEntry;
        MmsValue_setBinaryTime(timeOfEntry, currentTime);

        self->reportTime = currentTime + self->bufTm;
    }

    self->triggered = true;
}


//CHANGE _VER_0_3_3_
MmsTypeSpecification*
Reporting_createMmsBufferedRCBs(MmsMapping* self, MmsDomain* domain,
        LogicalNode* logicalNode, int reportsCount)
{
    MmsTypeSpecification* namedVariable = calloc(1,
            sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("BR");
    namedVariable->type = MMS_STRUCTURE;

    namedVariable->typeSpec.structure.elementCount = reportsCount;
    namedVariable->typeSpec.structure.elements = calloc(reportsCount,
            sizeof(MmsTypeSpecification*));

    int currentReport = 0;

    while (currentReport < reportsCount) {
        ReportControl* rc = ReportControl_create(true);

        rc->domain = domain;

        ReportControlBlockPtr reportControlBlock = getRCBForLogicalNodeWithIndex(
                self, logicalNode, currentReport, true);

        rc->name = createString(3, logicalNode->name, "$BR$",
                reportControlBlock->name);

        namedVariable->typeSpec.structure.elements[currentReport] =
                createBufferedReportControlBlock(reportControlBlock, rc);

        //rc->dataSet->elementCount;

        LinkedList_add(self->reportControls, rc);

        currentReport++;
    }

    return namedVariable;
}


//CHANGE _VER_0_3_3_
MmsTypeSpecification*
	Reporting_createMmsUnbufferedRCBs(MmsMapping* self, MmsDomain* domain,
		LogicalNode* logicalNode, int reportsCount) {
    
		MmsTypeSpecification* namedVariable = calloc(1,
            sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("RP");
    namedVariable->type = MMS_STRUCTURE;

    namedVariable->typeSpec.structure.elementCount = reportsCount;
    namedVariable->typeSpec.structure.elements = calloc(reportsCount,
            sizeof(MmsTypeSpecification*));

    int currentReport = 0;

    while (currentReport < reportsCount) {
        ReportControl* rc = ReportControl_create(false);

        rc->domain = domain;

        ReportControlBlockPtr reportControlBlock = getRCBForLogicalNodeWithIndex(
                self, logicalNode, currentReport, false);

        rc->name = createString(3, logicalNode->name, "$RP$",
                reportControlBlock->name);

        namedVariable->typeSpec.structure.elements[currentReport] =
                createUnbufferedReportControlBlock(reportControlBlock, rc);

        LinkedList_add(self->reportControls, rc);

        currentReport++;
    }

    return namedVariable;
}

MmsValueIndication
	Reporting_RCBWriteAccessHandler(MmsMapping* self, ReportControl* rc, char* elementName, MmsValue* value,
		MmsServerConnection* connection) {
    
		if (strcmp(elementName, "RptEna") == 0) {

      if (value->value.boolean == true) {

          if (rc->enabled == true)
              return MMS_VALUE_ACCESS_DENIED;

          //if (DEBUG) printf("Activate report for client %s\n",
          //        MmsServerConnection_getClientAddress(connection));

          MmsValue* dataSetValue;

          dataSetValue = ReportControl_getRCBValue(rc, "DatSet");

          if (dataSetValue != NULL ) {
              DataSetPtr dataSet = IedModel_lookupDataSet(
                      self->model,
                      dataSetValue->value.visibleString);

              rc->dataSet = dataSet;

              rc->inclusionField = MmsValue_newBitString(
                      dataSet->elementCount);

              rc->clientConnection = connection;

              // TODO check integrity bit in trigger options

              MmsValue* intgPd = ReportControl_getRCBValue(rc, "IntgPd");

              rc->intgPd = MmsValue_toUint32(intgPd);

              MmsValue* rptEna = ReportControl_getRCBValue(rc, "RptEna");

              MmsValue_update(rptEna, value);

              MmsValue* bufTm = ReportControl_getRCBValue(rc, "BufTm");

              rc->bufTm = MmsValue_toUint32(bufTm);
							
							rc->sqNum = 0;

              rc->inclusionFlags = calloc(dataSet->elementCount, sizeof(ReportInclusionFlag));

              rc->enabled = true;

              return MMS_VALUE_OK;
          }

          return MMS_VALUE_ACCESS_DENIED;
      } else {
          if ((rc->enabled) && (rc->clientConnection != connection))
              return MMS_VALUE_ACCESS_DENIED;

          //if (DEBUG) printf("Deactivate report for client %s\n",
          //                MmsServerConnection_getClientAddress(connection));

          free(rc->inclusionFlags);
          rc->inclusionFlags = NULL;

          rc->enabled = false;
      }

    }

    if (strcmp(elementName, "GI") == 0) {
      if ((rc->enabled) && (rc->clientConnection == connection)) {
          rc->gi = true;
          return MMS_VALUE_OK;
      }
    }

    

    if (rc->enabled == false) {
			
			MmsValue* rcbValue = ReportControl_getRCBValue(rc, elementName);
			
        if (rcbValue != NULL )
            MmsValue_update(rcbValue, value);
        else
            return MMS_VALUE_VALUE_INVALID;
    }
    else
        return MMS_VALUE_ACCESS_DENIED;

    return MMS_VALUE_OK;
}


void
Reporting_processReportEvents(MmsMapping* self, uint64_t currentTimeInMs)
{
    LinkedList element = self->reportControls;

    while ((element = LinkedList_getNext(element)) != NULL ) {
        ReportControl* rc = (ReportControl*) element->data;

        if (rc->enabled) {
            if (rc->gi) {
                updateTimeOfEntry(rc, currentTimeInMs);
                sendReport(rc, false, true);
                rc->triggered = false;
            }

            if (rc->intgPd > 0) {
                if (currentTimeInMs >= rc->nextIntgReportTime) {
                    rc->nextIntgReportTime = currentTimeInMs + rc->intgPd;
                    updateTimeOfEntry(rc, currentTimeInMs);
                    sendReport(rc, true, false);
                    rc->triggered = false;
                }
            }

            if (rc->triggered) {
                if (currentTimeInMs >= rc->reportTime) {
                    sendReport(rc, false, false);
                    rc->triggered = false;
                }
            }
        }
    }
}


/*******************************************************************************
	Locally viewed (static) routines definitions
*******************************************************************************/

static void
updateTimeOfEntry(ReportControl* self, uint64_t currentTime)
{
    MmsValue* timeOfEntry = self->timeOfEntry;
    MmsValue_setBinaryTime(timeOfEntry, currentTime);
}

static void
sendReport(ReportControl* self, bool isIntegrity, bool isGI)
{
    LinkedList reportElements = LinkedList_create();

    LinkedList deletableElements = LinkedList_create();

    MmsValue* rptId = ReportControl_getRCBValue(self, "RptID");
    MmsValue* optFlds = ReportControl_getRCBValue(self, "OptFlds");
    MmsValue* datSet = ReportControl_getRCBValue(self, "DatSet");

    LinkedList_add(reportElements, rptId);
    LinkedList_add(reportElements, optFlds);

    /* delete option fields for unsupported options */
    //MmsValue_setBitStringBit(optFlds, 2, false); /* report-time-stamp */
    MmsValue_setBitStringBit(optFlds, 5, false); /* data-reference */
    MmsValue_setBitStringBit(optFlds, 7, false); /* entryID */
    MmsValue_setBitStringBit(optFlds, 9, false); /* segmentation */

    MmsValue* sqNum = ReportControl_getRCBValue(self, "SqNum");

    if (MmsValue_getBitStringBit(optFlds, 1)) /* sequence number */
        LinkedList_add(reportElements, sqNum);

    if (MmsValue_getBitStringBit(optFlds, 2)) { /* report time stamp */
        LinkedList_add(reportElements, self->timeOfEntry);
    }

    if (MmsValue_getBitStringBit(optFlds, 4)) /* data set reference */
        LinkedList_add(reportElements, datSet);

    if (MmsValue_getBitStringBit(optFlds, 6)) { /* bufOvfl */
        MmsValue* bufOvfl = MmsValue_newBoolean(false);

        LinkedList_add(reportElements, bufOvfl);
        LinkedList_add(deletableElements, bufOvfl);
    }

    if (MmsValue_getBitStringBit(optFlds, 8))
        LinkedList_add(reportElements, self->confRev);

    if (isGI || isIntegrity)
        MmsValue_setAllBitStringBits(self->inclusionField);
    else
        MmsValue_deleteAllBitStringBits(self->inclusionField);

    LinkedList_add(reportElements, self->inclusionField);


    /* add data set value elements */
    int i = 0;
    for (i = 0; i < self->dataSet->elementCount; i++) {

        if (isGI || isIntegrity)
            LinkedList_add(reportElements, self->dataSet->fcda[i]->value);
        else {
            if (self->inclusionFlags[i] != REPORT_CONTROL_NONE) {
                LinkedList_add(reportElements, self->dataSet->fcda[i]->value);
                MmsValue_setBitStringBit(self->inclusionField, i, true);
            }
        }
    }

    if (MmsValue_getBitStringBit(optFlds, 3)) {
        for (i = 0; i < self->dataSet->elementCount; i++) {

            if (isGI || isIntegrity) {
                MmsValue* reason = MmsValue_newBitString(6);

                if (isGI)
                    MmsValue_setBitStringBit(reason, 5, true);

                if (isIntegrity)
                    MmsValue_setBitStringBit(reason, 4, true);

                LinkedList_add(reportElements, reason);
                LinkedList_add(deletableElements, reason);
            }
            else if (self->inclusionFlags[i] != REPORT_CONTROL_NONE) {
                MmsValue* reason = MmsValue_newBitString(6);

                switch(self->inclusionFlags[i]) {
                case REPORT_CONTROL_QUALITY_CHANGED:
                    MmsValue_setBitStringBit(reason, 2, true);
                    break;
                case REPORT_CONTROL_VALUE_CHANGED:
                    MmsValue_setBitStringBit(reason, 1, true);
                    break;
                case REPORT_CONTROL_VALUE_UPDATE:
                    MmsValue_setBitStringBit(reason, 3, true);
                    break;
                }

                LinkedList_add(reportElements, reason);
                LinkedList_add(deletableElements, reason);
            }
        }
    }

    MmsServerConnection_sendInformationReport(self->clientConnection, NULL,
            "RPT", reportElements);

    /* Increase sequence number */
    self->sqNum++;
    MmsValue_setUint16(sqNum, self->sqNum);

    LinkedList_destroyDeep(deletableElements, (void(*)(void*))MmsValue_delete);
    LinkedList_destroyStatic(reportElements);

    /* clear GI flag */
    if (isGI) {
        MmsValue* gi = ReportControl_getRCBValue(self, "GI");
        self->gi = false;
        MmsValue_setBoolean(gi, false);
    }
}


//CHANGE _VER_0_3_3_
static char*
createDataSetReference(ReportControlBlockPtr rcb, ReportControl* reportControl)
{
    char* dataSetReference;

    char* domainName = MmsDomain_getName(reportControl->domain);
    char* lnName = rcb->parent->name;

    dataSetReference = createString(5, domainName, "/", lnName, "$", rcb->dataSetName);

    return dataSetReference;
}


static MmsValue*
createOptFlds(ReportControlBlockPtr reportControlBlock)
{
    MmsValue* optFlds = MmsValue_newBitString(10);
    uint8_t options = reportControlBlock->options;

    if (options & 0x01)
        MmsValue_setBitStringBit(optFlds, 1, true);
    if (options & 0x02)
        MmsValue_setBitStringBit(optFlds, 2, true);
    if (options & 0x04)
        MmsValue_setBitStringBit(optFlds, 3, true);
    if (options & 0x08)
        MmsValue_setBitStringBit(optFlds, 4, true);
    if (options & 0x10)
        MmsValue_setBitStringBit(optFlds, 5, true);
    if (options & 0x20)
        MmsValue_setBitStringBit(optFlds, 6, true);
    if (options & 0x40)
        MmsValue_setBitStringBit(optFlds, 7, true);
    if (options & 0x80)
        MmsValue_setBitStringBit(optFlds, 8, true);

    return optFlds;
}

static MmsValue*
createTrgOps(ReportControlBlockPtr reportControlBlock) {
    MmsValue* trgOps = MmsValue_newBitString(6);

    uint8_t triggerOps = reportControlBlock->trgOps;

    if (triggerOps & 0x02)
        MmsValue_setBitStringBit(trgOps, 1, true);
    if (triggerOps & 0x04)
        MmsValue_setBitStringBit(trgOps, 2, true);
    if (triggerOps & 0x08)
        MmsValue_setBitStringBit(trgOps, 3, true);
    if (triggerOps & 0x10)
        MmsValue_setBitStringBit(trgOps, 4, true);
    if (triggerOps & 0x20)
        MmsValue_setBitStringBit(trgOps, 5, true);

    return trgOps;
}

static MmsTypeSpecification*
	createUnbufferedReportControlBlock(ReportControlBlockPtr reportControlBlock,
		ReportControl* reportControl) {
			
    MmsTypeSpecification* rcb = calloc(1, sizeof(MmsTypeSpecification));
    rcb->name = copyString(reportControlBlock->name);
    rcb->type = MMS_STRUCTURE;

    MmsValue* mmsValue = calloc(1, sizeof(MmsValue));
    mmsValue->deleteValue = false;
    mmsValue->type = MMS_STRUCTURE;
    mmsValue->value.structure.componentCount = 12;
    mmsValue->value.structure.components = calloc(12, sizeof(MmsValue*));

    rcb->typeSpec.structure.elementCount = 12;

    rcb->typeSpec.structure.elements = calloc(12,
            sizeof(MmsTypeSpecification*));

    MmsTypeSpecification* namedVariable = calloc(1,
            sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("RptID");
    namedVariable->typeSpec.visibleString = -129;
    namedVariable->type = MMS_VISIBLE_STRING;
    rcb->typeSpec.structure.elements[0] = namedVariable;
    mmsValue->value.structure.components[0] = MmsValue_newVisibleString(
            reportControlBlock->rptId);

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("RptEna");
    namedVariable->type = MMS_BOOLEAN;
    rcb->typeSpec.structure.elements[1] = namedVariable;
    mmsValue->value.structure.components[1] = MmsValue_newBoolean(false);

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("Resv");
    namedVariable->type = MMS_BOOLEAN;
    rcb->typeSpec.structure.elements[2] = namedVariable;
    mmsValue->value.structure.components[2] = MmsValue_newBoolean(false);

    char* dataSetReference = createDataSetReference(reportControlBlock,
            reportControl);

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("DatSet");
    namedVariable->typeSpec.visibleString = -129;
    namedVariable->type = MMS_VISIBLE_STRING;
    rcb->typeSpec.structure.elements[3] = namedVariable;
    mmsValue->value.structure.components[3] = MmsValue_newVisibleString(
            dataSetReference);
    free(dataSetReference);

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("ConfRev");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 32;
    rcb->typeSpec.structure.elements[4] = namedVariable;
    mmsValue->value.structure.components[4] =
            MmsValue_newUnsignedFromUint32(reportControlBlock->confRef);

    reportControl->confRev = mmsValue->value.structure.components[4];

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("OptFlds");
    namedVariable->type = MMS_BIT_STRING;
    namedVariable->typeSpec.bitString = 10;
    rcb->typeSpec.structure.elements[5] = namedVariable;
    mmsValue->value.structure.components[5] = createOptFlds(reportControlBlock);

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("BufTm");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 32;
    rcb->typeSpec.structure.elements[6] = namedVariable;
    mmsValue->value.structure.components[6] =
            MmsValue_newUnsignedFromUint32(reportControlBlock->bufferTime);

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("SqNum");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 16;
    rcb->typeSpec.structure.elements[7] = namedVariable;
    mmsValue->value.structure.components[7] = MmsValue_newUnsigned(16);

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("TrgOps");
    namedVariable->type = MMS_BIT_STRING;
    namedVariable->typeSpec.bitString = 6;
    rcb->typeSpec.structure.elements[8] = namedVariable;
    mmsValue->value.structure.components[8] = createTrgOps(reportControlBlock);

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("IntgPd");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 32;
    rcb->typeSpec.structure.elements[9] = namedVariable;
    mmsValue->value.structure.components[9] =
            MmsValue_newUnsignedFromUint32(reportControlBlock->intPeriod);

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("GI");
    namedVariable->type = MMS_BOOLEAN;
    rcb->typeSpec.structure.elements[10] = namedVariable;
    mmsValue->value.structure.components[10] = MmsValue_newBoolean(false);

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("Owner");
    namedVariable->type = MMS_OCTET_STRING;
    namedVariable->typeSpec.octetString = -128;
    rcb->typeSpec.structure.elements[11] = namedVariable;
    mmsValue->value.structure.components[11] = MmsValue_newOctetString(0, 128);

    reportControl->rcbValues = mmsValue;

    reportControl->timeOfEntry = MmsValue_newBinaryTime(false);

    return rcb;
}


static MmsTypeSpecification*
	createBufferedReportControlBlock(ReportControlBlockPtr reportControlBlock,
		ReportControl* reportControl) {
    
		MmsTypeSpecification* rcb = calloc(1, sizeof(MmsTypeSpecification));
    rcb->name = copyString(reportControlBlock->name);
    rcb->type = MMS_STRUCTURE;

    MmsValue* mmsValue = calloc(1, sizeof(MmsValue));
    mmsValue->deleteValue = false;
    mmsValue->type = MMS_STRUCTURE;
    mmsValue->value.structure.componentCount = 15;
    mmsValue->value.structure.components = calloc(15, sizeof(MmsValue*));

    rcb->typeSpec.structure.elementCount = 15;

    rcb->typeSpec.structure.elements = calloc(15,
            sizeof(MmsTypeSpecification*));

    MmsTypeSpecification* namedVariable = calloc(1,
            sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("RptID");
    namedVariable->typeSpec.visibleString = -129;
    namedVariable->type = MMS_VISIBLE_STRING;
    rcb->typeSpec.structure.elements[0] = namedVariable;
    mmsValue->value.structure.components[0] = MmsValue_newVisibleString(
            reportControlBlock->rptId);

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("RptEna");
    namedVariable->type = MMS_BOOLEAN;
    rcb->typeSpec.structure.elements[1] = namedVariable;
    mmsValue->value.structure.components[1] = MmsValue_newBoolean(false);

    char* dataSetReference = createDataSetReference(reportControlBlock,
            reportControl);

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("DatSet");
    namedVariable->typeSpec.visibleString = -129;
    namedVariable->type = MMS_VISIBLE_STRING;
    rcb->typeSpec.structure.elements[2] = namedVariable;
    mmsValue->value.structure.components[2] = MmsValue_newVisibleString(
            dataSetReference);
    free(dataSetReference);

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("ConfRev");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 32;
    rcb->typeSpec.structure.elements[3] = namedVariable;
    mmsValue->value.structure.components[3] =
            MmsValue_newUnsignedFromUint32(reportControlBlock->confRef);

    reportControl->confRev = mmsValue->value.structure.components[3];

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("OptFlds");
    namedVariable->type = MMS_BIT_STRING;
    namedVariable->typeSpec.bitString = 10;
    rcb->typeSpec.structure.elements[4] = namedVariable;
    mmsValue->value.structure.components[4] = createOptFlds(reportControlBlock);

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("BufTm");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 32;
    rcb->typeSpec.structure.elements[5] = namedVariable;
    mmsValue->value.structure.components[5] =
            MmsValue_newUnsignedFromUint32(reportControlBlock->bufferTime);

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("SqNum");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 16;
    rcb->typeSpec.structure.elements[6] = namedVariable;
    mmsValue->value.structure.components[6] = MmsValue_newUnsigned(16);

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("TrgOps");
    namedVariable->type = MMS_BIT_STRING;
    namedVariable->typeSpec.bitString = 6;
    rcb->typeSpec.structure.elements[7] = namedVariable;
    mmsValue->value.structure.components[7] = createTrgOps(reportControlBlock);

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("IntgPd");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 32;
    rcb->typeSpec.structure.elements[8] = namedVariable;
    mmsValue->value.structure.components[8] =
            MmsValue_newUnsignedFromUint32(reportControlBlock->intPeriod);

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("GI");
    namedVariable->type = MMS_BOOLEAN;
    rcb->typeSpec.structure.elements[9] = namedVariable;
    mmsValue->value.structure.components[9] = MmsValue_newBoolean(false);

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("PurgeBuf");
    namedVariable->type = MMS_BOOLEAN;
    rcb->typeSpec.structure.elements[10] = namedVariable;
    mmsValue->value.structure.components[10] = MmsValue_newBoolean(false);

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("EntryID");
    namedVariable->type = MMS_OCTET_STRING;
    namedVariable->typeSpec.octetString = 8;
    rcb->typeSpec.structure.elements[11] = namedVariable;
    mmsValue->value.structure.components[11] = MmsValue_newOctetString(8, 8);

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("TimeOfEntry");
    namedVariable->type = MMS_BINARY_TIME;
    rcb->typeSpec.structure.elements[12] = namedVariable;
    mmsValue->value.structure.components[12] = MmsValue_newBinaryTime(false);

    reportControl->timeOfEntry = mmsValue->value.structure.components[12];

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("ResvTms");
    namedVariable->type = MMS_UNSIGNED;
    namedVariable->typeSpec.unsignedInteger = 32;
    rcb->typeSpec.structure.elements[13] = namedVariable;
    mmsValue->value.structure.components[13] = MmsValue_newUnsigned(32);

    namedVariable = calloc(1, sizeof(MmsTypeSpecification));
    namedVariable->name = copyString("Owner");
    namedVariable->type = MMS_OCTET_STRING;
    namedVariable->typeSpec.octetString = -128;
    rcb->typeSpec.structure.elements[14] = namedVariable;
    mmsValue->value.structure.components[14] = MmsValue_newOctetString(0, 128);

    reportControl->rcbValues = mmsValue;

    return rcb;
}

//to delete
/*static GSEControlBlock*
getGCBForLogicalNodeWithIndex(MmsMapping* self, LogicalNode* logicalNode, int index)
{
    int gseCount = 0;
		int i = 0;

    GSEControBlockPtr* p_gseControlBlocks = self->model->gseCBs;
		GSEControlBlock* gseControlBlocks = (GSEControlBlock*)*(p_gseControlBlocks);
    

    while ((gseControlBlocks + i) != NULL ) {
        if ((gseControlBlocks + i)->parent == logicalNode) {
            if (gseCount == index)
                return (gseControlBlocks + i);

            gseCount++;
        }

        i++;
    }

    return NULL ;
}*/

static ReportControlBlockPtr
	getRCBForLogicalNodeWithIndex(MmsMapping* self, LogicalNode* logicalNode,
		int index, bool buffered) {
    
		int rcbCount = 0;

		ReportControlBlockPtr* p_reportControlBlocks = self->model->rcbs;

    int i = 0;

    while ( *(p_reportControlBlocks+i) != NULL ) {
        if ( ((ReportControlBlockPtr)*(p_reportControlBlocks+i))->parent == logicalNode) {
            if ( ((ReportControlBlockPtr)*(p_reportControlBlocks+i))->buffered == buffered) {
                if (rcbCount == index)
                    return (ReportControlBlockPtr)*(p_reportControlBlocks+i);

                rcbCount++;
            }

        }

        i++;
    }

    return NULL ;
}


