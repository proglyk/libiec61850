#ifndef __STM32F4xx_HAL_MSP_H
#define __STM32F4xx_HAL_MSP_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "FreeRTOSConfig.h"


/*******************************************************************************
	Interrupt Priorities
*******************************************************************************/
#define IRQPRIO_RTC							(-3 + configLIBRARY_LOWEST_INTERRUPT_PRIORITY)
#define IRQSUBPRIO_RTC					0

#define IRQPRIO_ETH							(-2 + configLIBRARY_LOWEST_INTERRUPT_PRIORITY)
#define IRQSUBPRIO_ETH					0

#define IRQPRIO_DEBUG						(0 + configLIBRARY_LOWEST_INTERRUPT_PRIORITY)
#define IRQSUBPRIO_DEBUG				(0 + 0xf)

#define IRQPRIO_WKUPKEY					(0 + configLIBRARY_LOWEST_INTERRUPT_PRIORITY)
#define IRQSUBPRIO_WKUPKEY			(0 + 0xf)




#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_MSP_H */
