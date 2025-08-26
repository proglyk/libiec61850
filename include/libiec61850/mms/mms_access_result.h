#ifndef MMS_ACCESS_RESULT_H_
#define MMS_ACCESS_RESULT_H_

#include "libiec61850/mms/mms_value.h"

int
mmsServer_encodeAccessResult(MmsValue* value, uint8_t* buffer, int bufPos, bool encode);

#endif /* MMS_ACCESS_RESULT_H_ */
