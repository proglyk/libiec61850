#ifndef _MMS_INFORMATION_REPORT_H_
#define _MMS_INFORMATION_REPORT_H_

void
MmsServerConnection_sendInformationReport(MmsServerConnection* self, char* domainId, char* itemId, LinkedList values);

#endif