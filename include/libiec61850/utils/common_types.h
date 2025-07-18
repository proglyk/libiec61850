#ifndef _COMMON_TYPES_H_
#define _COMMON_TYPES_H_

#include "libiec61850/utils/byte_buffer.h"
#include "libiec61850/utils/SBuffer.h"

typedef void (*PassedHandlerPtr) ( void *parameter,
                                      ByteBuffer* request, SBuffer *response );

#endif // _COMMON_TYPES_H_