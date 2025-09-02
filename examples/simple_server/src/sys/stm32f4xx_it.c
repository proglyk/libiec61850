#include "stm32f4xx_it.h"
#include "stm32f4xx_hal_msp.h"
#include "net.h"

#if (defined _TEST_) & (defined SERV_IS_407IC)
extern UART_HandleTypeDef * pxDebug;
#endif //_TEST_

#if (defined _TEST_) & (defined SERV_IS_407IC)
void
USART3_IRQHandler(void) {
	HAL_UART_IRQHandler(pxDebug);
}
#endif //_TEST_,SERV_IS_407IC

extern void	xPortSysTickHandler(void);

/**	----------------------------------------------------------------------------
  * @brief  This function handles TIM interrupt request. */
void 
  ETH_IRQHandler(void) {
/*----------------------------------------------------------------------------*/
	net__irq(net__inst());
}

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
//void
//	KEY_BUTTON_EXTI_IRQHandler(void) {
/*----------------------------------------------------------------------------*/
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void
  SysTick_Handler(void) {
/*----------------------------------------------------------------------------*/
	xPortSysTickHandler();
}


/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}







