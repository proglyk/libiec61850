#include "stm32f4xx_it_cb.h"
//#include "net.h"
#include "proj_conf.h"
#include "cmsis_os.h"

volatile uint32_t ulHighFrequencyTimerCounts;
#if (defined _TEST_) & (defined SERV_IS_407IC)
extern UART_HandleTypeDef * pxDebug;
extern osMessageQId message_q_id;
extern u8_t inbox[32];
#endif //_TEST_

/**	----------------------------------------------------------------------------
	* @brief обработчик прерывания по приему can-сообщения с линии
	* @param CanHandle: указатель на упр.стр.
	* @retval none: Нет */
/*void
HAL_CAN_RxCpltCallback(CAN_HandleTypeDef *CanHandle) {
//------------------------------------------------------------------------------
	// если пришло сообщение с Id STD и конвертор настроен на STANDARD, то
	if (CanHandle->pRxMsg->IDE == CAN_ID_STD) {
		// Мигаем
		
		__NOP(); }
	
	HAL_CAN_Receive_IT(CanHandle, CAN_FIFO0);
}*/

void
	HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart) {
#if (defined _TEST_) & (defined SERV_IS_407IC)
	if(huart->Instance == pxDebug->Instance) {
		osMessagePut(message_q_id, (uint32_t)inbox[0], osWaitForever);
	}
	// Для всех случаев прин¤того байта вновь включить UART в работу
	HAL_UART_Receive_IT(huart, (uint8_t *)inbox, 1);
#endif //_TEST_
}


void
  HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
		
	if (htim->Instance == TIM6)	{
		HAL_IncTick();
	}
}


void
  HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth) {
		
	//net__input(net__inst());
}

