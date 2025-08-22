#include "stm32f4xx_hal.h"
#include "userlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "userint.h"
#include "net.h"

static int internet_config( void );

// Внешний код - Сервер UPVS
extern net_if_fn_t xFnSrvIec61850;
//static IsoServerPld_t xPldMms;
// static IsoServerPld_t xPldMms = {
//   .pvMmsHandler = isoConnectionIndicationHandler,
//   .pvMmsPayload = NULL
// };

static void task_main(void *argument);
static void SystemClock_Config( void );

// Область памяти RAM, выделенная для раcположения в ней HEAP FreeRTOS
#pragma data_alignment=4 
__ALIGN_BEGIN
__attribute__((__section__(".heap"))) uint8_t ucHeap[ configTOTAL_HEAP_SIZE ]
__ALIGN_END;

// Область памяти RAM, выделенная для раcположения в ней HEAP LwIP
#pragma data_alignment=4 
__ALIGN_BEGIN
__attribute__((__section__(".heap"))) uint8_t lwip_heap_user[MEM_SIZE + 0x14]
__ALIGN_END;

u8_t inbox[32]; // для it_cb.c

/**	----------------------------------------------------------------------------
	* @brief Точка входа в программу */
int main(void) {
/*----------------------------------------------------------------------------*/
  TaskHandle_t xTask;
  
  HAL_Init();
	SystemClock_Config();
  
  internet_config();
  
  xTaskCreate( task_main, 
               "main", 
               4*configMINIMAL_STACK_SIZE, 
               NULL, 
               uxGetPriority(PRIO_IDLE), 
               &(xTask) );

  // планировщик
	vTaskStartScheduler();
  
	for (;;) { }
}

/**	----------------------------------------------------------------------------
	* @brief  Запуск очередного измерения АЦП, работа регулятора, проверка связи
	* @retval none: Нет */
static void
	task_main(void *argument) {
/*----------------------------------------------------------------------------*/   
  //GPIOE->ODR &= ~(1<<6);
  // задержка / переключение контекста
  vTaskDelay(100);
}

/**	----------------------------------------------------------------------------
	* @brief  Контроль выделения стэка
	* @param xTask: Указатель на задачу
	* @param pcTaskName: Имя задачи */
void
	vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName ) {
/*----------------------------------------------------------------------------*/	
	while (1){
		asm("	nop");
	}
}

/**	----------------------------------------------------------------------------
	* @brief  Контроль выделения памяти malloc */
void
	vApplicationMallocFailedHook( void ) {
/*----------------------------------------------------------------------------*/
	while (1){
		asm("	nop");
	}
}

/**	----------------------------------------------------------------------------
	* @brief  Инициализация тактирования кристалла */
static void 
  SystemClock_Config(void) {
/*----------------------------------------------------------------------------*/
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  // Initializes the CPU, AHB and APB busses clocks 
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  // Настройка на 168 МГц
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  // только для настрйоки платы как источника сигнала 50 МГц для PHY
  //RCC_OscInitStruct.PLL.PLLM = 4;
  //RCC_OscInitStruct.PLL.PLLN = 150;
  //RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  //RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)  {
    for(;;)	{ __NOP(); }
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
    for(;;) { __NOP(); }
  }
	
	// Output SYSCLK divided by 3 on MCO1 pin(PA8)
  //HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_3);
}

static int internet_config(void) {
  s32_t rc;
	net_init_t init_str;

	//
	rc = net__init(net__inst());
  if (rc<0) {
    //LWIP_DEBUGF( NET_DEBUG, ("ETH couldn't be started, in '%s' /NET/net_netif.c:%d\r\n", 
    //  __FUNCTION__, __LINE__) );
    goto exit;
  }
  
  //
  //xPldMms.pvMmsHandler = isoConnectionIndicationHandler;
  //xPldMms.pvMmsPayload = NULL;
	//
	init_str.pcName = "iec61850_srv";
	init_str.pxFn = &xFnSrvIec61850;
  init_str.bEnabled = true;
	init_str.ulPort = 102;
  init_str.pvTopPld = NULL; //(void *)&xPldMms;
	rc = net__add_srv(net__inst(), &init_str);
  if (rc < 0) {
    //LWIP_DEBUGF( NET_DEBUG, ("\"%s\" hasn't added...Err=%01d, in '%s' /main.c:%d\r\n", 
    //  init_str.pcName, rc, __FUNCTION__, __LINE__) );
    goto exit;
  }
  
	// запуск
	net__run(net__inst());
  return 0;
  
exit:
  return -1;
}
