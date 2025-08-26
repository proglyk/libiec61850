#ifndef REPORTING_H_
#define REPORTING_H_

typedef struct {
    char* name;
    MmsDomain* domain;
    MmsValue* rcbValues;
    MmsValue* inclusionField;
    MmsValue* confRev;
    MmsValue* timeOfEntry;
    DataSetPtr dataSet;
    bool enabled;
    bool bufferd;
    bool gi;
    bool triggered;
    uint16_t sqNum;
    uint32_t intgPd;
    uint32_t bufTm;
    uint64_t nextIntgReportTime;
    uint64_t reportTime;
    MmsServerConnection* clientConnection;
    ReportInclusionFlag* inclusionFlags;
} ReportControl;

ReportControl*
ReportControl_create(bool buffered);

void
ReportControl_destroy(ReportControl* self);

void
ReportControl_valueUpdated(ReportControl* self, int dataSetEntryIndex, ReportInclusionFlag flag);

MmsValue*
ReportControl_getRCBValue(ReportControl* rc, char* elementName);

MmsTypeSpecification*
Reporting_createMmsBufferedRCBs(MmsMapping* self, MmsDomain* domain,
        LogicalNode* logicalNode, int reportsCount);

MmsTypeSpecification*
Reporting_createMmsUnbufferedRCBs(MmsMapping* self, MmsDomain* domain,
        LogicalNode* logicalNode, int reportsCount);

MmsValueIndication
Reporting_RCBWriteAccessHandler(MmsMapping* self, ReportControl* rc, char* elementName, MmsValue* value,
        MmsServerConnection* connection);
				
void
Reporting_processReportEvents(MmsMapping* self, uint64_t currentTimeInMs);

#endif /* REPORTING_H_ */
