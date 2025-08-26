#include "libiec61850/api/mms_mapping/control.h"
#include "libiec61850/mms/mms_value.h"
#include "hal.h"
#include "time_hal.h"
//#include "heap_user.h"


struct sControlObject {
    MmsDomain* mmsDomain;
    MmsServer mmsServer;
    char* lnName;
    char* name;

    MmsValue* mmsValue;

    MmsValue* oper;
    MmsValue* sbo;
    MmsValue* sbow;
    MmsValue* cancel;

    MmsValue* ctlVal;
    MmsValue* ctlNum;
    MmsValue* origin;
    MmsValue* timestamp;

    bool selected;
    uint64_t selectTime;
    uint32_t selectTimeout;

    bool initialized;
    uint32_t ctlModel;

    ControlHandler listener;
    void* listenerParameter;
};

static void
initialize(ControlObject* self)
{
    if (!(self->initialized)) {
        char* ctlModelName = createString(4, self->lnName, "$CF$", self->name, "$ctlModel");

        MmsValue* ctlModel = MmsServer_getValueFromCache(self->mmsServer,
                self->mmsDomain, ctlModelName);

        free(ctlModelName);

        if (ctlModel != NULL) {
            uint32_t ctlModelVal = MmsValue_toInt32(ctlModel);

            self->ctlModel = ctlModelVal;

            if ((ctlModelVal == 2) || (ctlModelVal == 4)) { /* SBO */
                char* sboTimeoutName = createString(4, self->lnName, "$CF$", self->name, "$sboTimeout");

                MmsValue* sboTimeout = MmsServer_getValueFromCache(self->mmsServer,
                                self->mmsDomain, sboTimeoutName);

                free(sboTimeoutName);

                if (sboTimeout != NULL) {
                    uint32_t sboTimeoutVal = MmsValue_toInt32(sboTimeout);

                    self->selectTimeout = sboTimeoutVal;
                }
            }
        }

        self->initialized = true;
    }
}

ControlObject*
ControlObject_create(MmsServer mmsServer, MmsDomain* domain, char* lnName, char* name)
{
    ControlObject* self = calloc(1, sizeof(ControlObject));

    self->name = name;
    self->lnName = lnName;
    self->mmsDomain = domain;
    self->mmsServer = mmsServer;

    return self;
}

void
ControlObject_setOper(ControlObject* self, MmsValue* oper)
{
    self->oper = oper;
}

void
ControlObject_setCancel(ControlObject* self, MmsValue* cancel)
{
    self->cancel = cancel;
}

void
ControlObject_setSBO(ControlObject* self, MmsValue* sbo)
{
    self->sbo = sbo;
}

void
ControlObject_setSBOw(ControlObject* self, MmsValue* sbow)
{
    self->sbow = sbow;
}

void
ControlObject_setCtlVal(ControlObject* self, MmsValue* ctlVal)
{
    self->ctlVal = ctlVal;
}

char*
ControlObject_getName(ControlObject* self)
{
    return self->name;
}

char*
ControlObject_getLNName(ControlObject* self)
{
    return self->lnName;
}

MmsDomain*
ControlObject_getDomain(ControlObject* self)
{
    return self->mmsDomain;
}

MmsValue*
ControlObject_getOper(ControlObject* self)
{
    return self->oper;
}

MmsValue*
ControlObject_getSBOw(ControlObject* self)
{
    return self->sbow;
}

MmsValue*
ControlObject_getSBO(ControlObject* self)
{
    return self->sbo;
}

MmsValue*
ControlObject_getCancel(ControlObject* self)
{
    return self->cancel;
}

void
ControlObject_setMmsValue(ControlObject* self, MmsValue* value)
{
    self->mmsValue = value;
}

MmsValue*
ControlObject_getMmsValue(ControlObject* self)
{
    return self->mmsValue;
}

static void
selectObject(ControlObject* self, uint64_t selectTime)
{
    self->selected = true;
    self->selectTime = selectTime;
}

static bool
isSelected(ControlObject* self, uint64_t currentTime)
{
    if ((self->ctlModel == 2) || (self->ctlModel == 4)) {

        if (self->selected) {

            if (currentTime > (self->selectTime + self->selectTimeout)) {
                self->selected = false;
                return false;
            }

            return true;
        }
        else
            return false;
    }
    else
        return true;
}

bool
ControlObject_select(ControlObject* self)
{
    initialize(self);

    uint64_t currentTime = Hal_getTimeInMs();

    if (isSelected(self, currentTime))
        return false;
    else
        selectObject(self, currentTime);

    return true;
}

bool
ControlObject_operate(ControlObject* self, MmsValue* value)
{
    initialize(self);

    uint64_t currentTime = Hal_getTimeInMs();

    if (isSelected(self, currentTime)) {
        self->selectTime = currentTime;

        if (self->listener != NULL) {
            self->listener(self->listenerParameter, value);
        }

        return true;
    }

    return false;
}

bool
ControlObject_installListener(ControlObject* self, ControlHandler listener, void* parameter)
{
    self->listener = listener;
    self->listenerParameter = parameter;
		
		return 1;
}


void
ControlObject_destroy(ControlObject* self)
{
    if (self->mmsValue != NULL)
        MmsValue_delete(self->mmsValue);

    free(self);
}
