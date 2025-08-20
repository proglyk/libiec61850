#include "stm32f4xx.h"
#include "stm32f4xx_hal_msp.h"
#include "proj_conf.h"
//#include "upvs_config.h"

#if (defined _TEST_) & (defined SERV_IS_407IC)
extern UART_HandleTypeDef * pxDebug;
extern void debug__MspInit(UART_HandleTypeDef* huart);
#endif //_TEST_

/**	----------------------------------------------------------------------------
	* @brief  Функция HAL_MspInit
	* @retval none: Нет */
void
  HAL_MspInit(void) {
/*----------------------------------------------------------------------------*/
	GPIO_InitTypeDef  xGpioSetup;
  
#if (defined SERV_IS_407IC)
  
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  
  // Мигалки
  xGpioSetup.Mode      = GPIO_MODE_OUTPUT_PP;
	xGpioSetup.Pull      = GPIO_NOPULL;
	xGpioSetup.Speed     = GPIO_SPEED_FREQ_HIGH;
	xGpioSetup.Pin       = (1<<2) | (1<<3);
	HAL_GPIO_Init(GPIOH, &xGpioSetup);
	xGpioSetup.Pin       = (1<<8) | (1<<10);
	HAL_GPIO_Init(GPIOI, &xGpioSetup);

  xGpioSetup.Mode      = GPIO_MODE_OUTPUT_PP;
	xGpioSetup.Pull      = GPIO_NOPULL;
	xGpioSetup.Speed     = GPIO_SPEED_FREQ_HIGH;
  xGpioSetup.Pin       = (1<<2) | (1<<6);
	HAL_GPIO_Init(GPIOE, &xGpioSetup);
  
#elif (defined SERV_IS_PI396)

	__HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
  
  // Зеленая мигалка
  xGpioSetup.Pin       = (1<<1) | (1<<0);
	xGpioSetup.Mode      = GPIO_MODE_OUTPUT_PP;
	xGpioSetup.Pull      = GPIO_NOPULL;
	xGpioSetup.Speed     = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE, &xGpioSetup);
  
  xGpioSetup.Pin       = (1<<8) | (1<<9);
	HAL_GPIO_Init(GPIOB, &xGpioSetup);
  
  // Configure PA6 as PHY_NRST (до HAL_ETH_MspInit т.к. сбрасывает раньше Init)
  xGpioSetup.Speed = GPIO_SPEED_HIGH;
  xGpioSetup.Mode = GPIO_MODE_OUTPUT_OD;
  xGpioSetup.Pull = GPIO_PULLUP;
  xGpioSetup.Pin = GPIO_PIN_6;
  HAL_GPIO_Init(GPIOA, &xGpioSetup);
  
#endif // SERV_IS_407IC,SERV_IS_PI396
  
}

#ifdef _TEST_
/**	----------------------------------------------------------------------------
	* @brief  Функция HAL_CAN_MspInit
	* @retval none: Нет */
void
  HAL_CAN_MspInit(CAN_HandleTypeDef *hcan) {
//------------------------------------------------------------------------------
	GPIO_InitTypeDef  xGpioSetup;
  
  if (hcan->Instance == CAN1) {

#if (defined SERV_IS_407IC)

    __HAL_RCC_CAN1_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    xGpioSetup.Pin       = (1<<9);
    xGpioSetup.Mode      = GPIO_MODE_AF_PP;
    xGpioSetup.Pull      = GPIO_PULLUP;
    xGpioSetup.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    xGpioSetup.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOI, &xGpioSetup);
    xGpioSetup.Pin       = (1<<13);
    HAL_GPIO_Init(GPIOH, &xGpioSetup);

    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 1, 0);
    NVIC_EnableIRQ(CAN1_RX1_IRQn);

      
#elif (defined SERV_IS_PI396)
      
    __HAL_RCC_CAN1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    xGpioSetup.Pin       = (1<<11)|(1<<12);
    xGpioSetup.Mode      = GPIO_MODE_AF_PP;
    xGpioSetup.Pull      = GPIO_PULLUP;
    xGpioSetup.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    xGpioSetup.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &xGpioSetup);

    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 1, 0);
    NVIC_EnableIRQ(CAN1_RX1_IRQn);
    
#endif // SERV_IS_407IC,SERV_IS_PI396
    
  }
  
  if (hcan->Instance == CAN2) {
    
#if ((defined SERV_IS_407IC) || (defined SERV_IS_PI396))
    
    __HAL_RCC_CAN2_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    xGpioSetup.Pin       = (1<<5)|(1<<6);
    xGpioSetup.Mode      = GPIO_MODE_AF_PP;
    xGpioSetup.Pull      = GPIO_PULLUP;
    xGpioSetup.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    xGpioSetup.Alternate = GPIO_AF9_CAN2;
    HAL_GPIO_Init(GPIOB, &xGpioSetup);
    
    HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN2_SCE_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(CAN2_SCE_IRQn);  
    
#endif

  }
}
#endif // _TEST_

/**	----------------------------------------------------------------------------
	* @brief  ??? */
void
	HAL_UART_MspInit(UART_HandleTypeDef *huart) {
/*----------------------------------------------------------------------------*/
#if (defined _TEST_) & (defined SERV_IS_407IC)
	if (huart->Instance == pxDebug->Instance) {
		debug__MspInit(huart);
	}
#endif //_TEST_
}

void
HAL_ETH_MspInit(ETH_HandleTypeDef *heth) {
/*----------------------------------------------------------------------------*/
  GPIO_InitTypeDef GPIO_InitStructure;

#if (defined SERV_IS_407IC)

  /* Enable GPIOs clocks */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

/* Ethernet pins configuration ************************************************/
// Плата Open407I-C
/*
  не исп.     PA8           OK не исп. закорочен с PA1
  REF_CLK     PA1           OK исп. как вход для так. частоты
	RMII_MDIO		PA2						OK
	RMII_MDC		PC1						OK
	RMII_CRS_DV	PA7						OK
	RMII_RXD0		PC4						OK
	RMII_RXD1		PC5						OK
	RMII_TX_EN	PG11					OK
	RMII_TXD0		PG13					OK
	RMII_TXD1		PG14					OK
  +
  RMII_NSRT		PA3					  OK (должен настр. в HAL_MspInit)
*/
  
  // Configure PA1
  /*GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructure.Pull = GPIO_PULLDOWN;
  GPIO_InitStructure.Pin = GPIO_PIN_1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);*/

  // Configure PA2, PA1
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Alternate = GPIO_AF11_ETH;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_2;	//2 перенести
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Configure PA7 */
	GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Pin = GPIO_PIN_7;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure PC1 */
	GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_1;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  /* Configure PC4, PC5 */
	GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Pin = GPIO_PIN_4 | GPIO_PIN_5;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Configure PG2, PG11, PG13 and PG14 */
	GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin =  GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

  /* Enable the Ethernet global Interrupt */
  HAL_NVIC_SetPriority(ETH_IRQn, IRQPRIO_ETH, IRQSUBPRIO_ETH);
  HAL_NVIC_EnableIRQ(ETH_IRQn);

  /* Enable ETHERNET clock  */
  __HAL_RCC_ETH_CLK_ENABLE();
  
#elif (defined SERV_IS_PI396)
  
  /* Enable GPIOs clocks */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  //__HAL_RCC_GPIOG_CLK_ENABLE();

/* Ethernet pins configuration ************************************************/
  /*
        
        RMII_REF_CLK ----------------------> PA1 ок   alt
        RMII_MDIO -------------------------> PA2 ок   alt
        ETH_INT   -------------------------> PA5 нет  in
        ETH_RESET -------------------------> PA6 нет  out Настроен в HAL_MspInit
        RMII_MII_CRS_DV -------------------> PA7 ок   alt
        
        RMII_MDC --------------------------> PC1 ок   alt
        RMII_MII_RXD0 ---------------------> PC4 ок   alt
        RMII_MII_RXD1 ---------------------> PC5 ок   alt
        
        
        RMII_MII_TX_EN --------------------> PB11 ок  alt
        RMII_MII_TXD0 ---------------------> PB12 ок  alt
        RMII_MII_TXD1 ---------------------> PB13 ок  alt
        
        RMII_MII_RXER ---------------------> PG2 - not used
  */

  // Configure PA1, PA2. No internal pull-up resistors
  GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_2;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Alternate = GPIO_AF11_ETH;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  // PA7. External 4.7k + internal pull-up
  GPIO_InitStructure.Pin = GPIO_PIN_7;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  // PC1
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_1;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
  // PC4, PC5
	GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Pin = GPIO_PIN_4 | GPIO_PIN_5;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  // Configure PB11, PB12 and PB13
  GPIO_InitStructure.Pin =  GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  // PA5
  GPIO_InitStructure.Pull = GPIO_PULLDOWN;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructure.Pin = GPIO_PIN_5;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  

  // Enable the Ethernet global Interrupt
  HAL_NVIC_SetPriority(ETH_IRQn, IRQPRIO_ETH, IRQSUBPRIO_ETH);
  HAL_NVIC_EnableIRQ(ETH_IRQn);

  /* Enable ETHERNET clock  */
  __HAL_RCC_ETH_CLK_ENABLE();
  
#endif // SERV_IS_407IC,SERV_IS_407IC
}

/**	----------------------------------------------------------------------------
	* @brief  Функция HAL_ETH_MspDeInit
	* @retval none: Нет */
void
HAL_ETH_MspDeInit(ETH_HandleTypeDef *heth) {
/*----------------------------------------------------------------------------*/
	/* Disable the Ethernet global Interrupt */
	HAL_NVIC_DisableIRQ(ETH_IRQn);
	
	/* Disable ETHERNET clock  */
	__HAL_RCC_ETH_CLK_DISABLE();
}