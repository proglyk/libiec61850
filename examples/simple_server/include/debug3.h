#ifndef _DEBUG3_H_
#define _DEBUG3_H_

#include "stm32f4xx_hal.h"
// #include <stdbool.h>
#include "userio.h"
#include "userint.h"
#include <stdarg.h>

// bool debug__set_baudrate(u32_t);
void debug__init(u32_t *, IRQn_Type);
// osEvent debug__message_get(u32_t);


#endif /*_DEBUG3_H_*/
