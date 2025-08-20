#include "stm32f4xx_hal.h"


void TIM6_DAC_IRQHandler(void);


static TIM_HandleTypeDef TimHandle;


HAL_StatusTypeDef 
	HAL_InitTick (uint32_t TickPriority) {
		
  RCC_ClkInitTypeDef    clkconfig;
  uint32_t              uwTimclock, uwAPB1Prescaler = 0U;
  uint32_t              uwPrescalerValue = 0U;
  uint32_t              pFLatency;
  
    /*Configure the TIM6 IRQ priority */
  HAL_NVIC_SetPriority(TIM6_DAC_IRQn, TickPriority ,0U);
  
  /* Enable the TIM6 global Interrupt */
  HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
  
  /* Enable TIM6 clock */
  __HAL_RCC_TIM6_CLK_ENABLE();
  
  /* Get clock configuration */
  HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);
  
  /* Get APB1 prescaler */
  uwAPB1Prescaler = clkconfig.APB1CLKDivider;
  
  /* Compute TIM6 clock */
  if (uwAPB1Prescaler == RCC_HCLK_DIV1) 
  {
    uwTimclock = HAL_RCC_GetPCLK1Freq();
  }
  else
  {
    uwTimclock = 2*HAL_RCC_GetPCLK1Freq();
  }
  
  /* Compute the prescaler value to have TIM6 counter clock equal to 1MHz */
  uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);
  
  /* Initialize TIM6 */
  TimHandle.Instance = TIM6;
  
  /* Initialize TIMx peripheral as follow:
  + Period = [(TIM6CLK/1000) - 1]. to have a (1/1000) s time base.
  + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
  + ClockDivision = 0
  + Counter direction = Up
  */
  TimHandle.Init.Period = (1000000U / 1000U) - 1U;
  TimHandle.Init.Prescaler = uwPrescalerValue;
  TimHandle.Init.ClockDivision = 0;
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	
  if(HAL_TIM_Base_Init(&TimHandle) == HAL_OK) {
    return HAL_TIM_Base_Start_IT(&TimHandle);
  }
  
  return HAL_ERROR;
}


void HAL_SuspendTick(void)
{
  __HAL_TIM_DISABLE_IT(&TimHandle, TIM_IT_UPDATE);
}


void HAL_ResumeTick(void)
{
  __HAL_TIM_ENABLE_IT(&TimHandle, TIM_IT_UPDATE);
}


void TIM6_DAC_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&TimHandle);
}
