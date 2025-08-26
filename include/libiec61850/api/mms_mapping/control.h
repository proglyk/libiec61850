#ifndef CONTROL_H_
#define CONTROL_H_

#include "libiec61850/api/model.h"
#include "libiec61850/mms/mms_server_conn.h"
#include "libiec61850/mms/mms_device_model.h"
#include "libiec61850/api/ied_server_api.h"

typedef struct sControlObject ControlObject;

ControlObject*
ControlObject_create(MmsServer mmsServer, MmsDomain* domain, char* lnName, char* name);

void
ControlObject_setOper(ControlObject* self, MmsValue* oper);

void
ControlObject_setCancel(ControlObject* self, MmsValue* cancel);

void
ControlObject_setSBO(ControlObject* self, MmsValue* sbo);

void
ControlObject_setSBOw(ControlObject* self, MmsValue* sbow);

void
ControlObject_setMmsValue(ControlObject* self, MmsValue* value);

MmsValue*
ControlObject_getMmsValue(ControlObject* self);

MmsValue*
ControlObject_getOper(ControlObject* self);

MmsValue*
ControlObject_getSBOw(ControlObject* self);

MmsValue*
ControlObject_getSBO(ControlObject* self);

MmsValue*
ControlObject_getCancel(ControlObject* self);

void
ControlObject_setCtlVal(ControlObject* self, MmsValue* ctlVal);

char*
ControlObject_getName(ControlObject* self);

char*
ControlObject_getLNName(ControlObject* self);

MmsDomain*
ControlObject_getDomain(ControlObject* self);

bool
ControlObject_select(ControlObject* self);

bool
ControlObject_operate(ControlObject* self, MmsValue* value);

bool
ControlObject_installListener(ControlObject* self, ControlHandler listener, void* parameter);

void
ControlObject_destroy(ControlObject* self);

#endif /* CONTROL_H_ */
