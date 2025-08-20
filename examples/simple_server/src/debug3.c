#include "debug3.h"
#include "proj_conf.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "cmsis_os.h"
#include <stdbool.h>
#include "stm32f4xx_hal_msp.h"
#include <string.h>

// WARN условие исп uprintf выставляется флагом DBG_ON
//#if (defined _TEST_) & (defined SERV_IS_407IC)

#define USART_TXBUFSIZE                 128
#define USART_TXBUFMASK                 (USART_TXBUFSIZE-1)
//#define USART_RXBUFSIZE               128
//#define USART_RXBUFMASK               (USART_RXBUFSIZE-1)
#define UPRINTF_BUF_SIZE                USART_TXBUFSIZE

static void task_print( void * );
static void periph_init(UART_HandleTypeDef* huart);
void debug__MspInit(UART_HandleTypeDef *);

static void usartputc(char c);
static void usartputs(const char *c);

bool periph_initialised = false;
static u8_t aRxBuffer[1] = {0};
osMessageQId message_q_id;
static osMessageQDef(message_q, 16, u8_t);

static TaskHandle_t pvTaskPrint; // TODO переделать на поле структуры
static u16_t usartinTX = 0;
static u16_t usartoutTX = 0;
char usartbuftx[USART_TXBUFSIZE];
static char strbuf[UPRINTF_BUF_SIZE];
SemaphoreHandle_t usarttxSemaphore;

static UART_HandleTypeDef uart_st = {
	.Instance = NULL,
	.Init = {
		.BaudRate = 115200,
		.WordLength = UART_WORDLENGTH_8B,
		.StopBits = UART_STOPBITS_2,
		.Parity = UART_PARITY_NONE,
		.Mode = UART_MODE_TX_RX,
		.HwFlowCtl = UART_HWCONTROL_NONE,
		.OverSampling = UART_OVERSAMPLING_16
	},
};
UART_HandleTypeDef * pxDebug = &uart_st;

static struct {
//	UART_HandleTypeDef uart;
	IRQn_Type irq;
} debug;

/**	----------------------------------------------------------------------------
	* @brief ??? */
void
debug__init(u32_t* instance, IRQn_Type irq) {
/*----------------------------------------------------------------------------*/
	message_q_id = osMessageCreate(osMessageQ(message_q), NULL);

	pxDebug->Instance = (USART_TypeDef*)instance;
	debug.irq = irq;
	HAL_UART_Init(pxDebug);
	HAL_UART_Receive_IT(pxDebug, (u8_t *)aRxBuffer, 1);
  
  // usarttxSemaphore = xSemaphoreCreateBinary();
  
  // xTaskCreate( task_print, 
               // "Print", 
               // 2*configMINIMAL_STACK_SIZE, 
               // NULL, 
               // makeFreeRtosPriority(osPriorityNormal), 
               // &(pvTaskPrint) );
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
void
debug__MspInit(UART_HandleTypeDef *huart) {
/*----------------------------------------------------------------------------*/
	periph_init(huart);
	periph_initialised = true;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static void
periph_init(UART_HandleTypeDef* huart) {
/*----------------------------------------------------------------------------*/
	GPIO_InitTypeDef Gpio;

	__HAL_RCC_USART3_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	Gpio.Pin = (1<<10);
	Gpio.Mode = GPIO_MODE_AF_PP;
	Gpio.Speed = GPIO_SPEED_FAST;
	Gpio.Pull = GPIO_NOPULL;
	Gpio.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(GPIOC, &Gpio);

	Gpio.Pin = (1<<11);
	Gpio.Mode = GPIO_MODE_AF_PP;
	Gpio.Speed = GPIO_SPEED_FAST;
	Gpio.Pull = GPIO_NOPULL;
	Gpio.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(GPIOC, &Gpio);

	HAL_NVIC_SetPriority(debug.irq, IRQPRIO_DEBUG, IRQSUBPRIO_DEBUG);
	HAL_NVIC_EnableIRQ(debug.irq);
}

// /**	----------------------------------------------------------------------------
	// * @brief ??? */
// void
	// debug3__start( net_netif_t *px ) {
// /*----------------------------------------------------------------------------*/
	// xTaskCreate( task_print, 
               // "Print", 
               // 2*configMINIMAL_STACK_SIZE, 
               // px, 
               // makeFreeRtosPriority(osPriorityNormal), 
               // &(pvTaskPrint) );
// }

/**	----------------------------------------------------------------------------
	* @brief ??? */
static void
	task_print( void *pargv ) {
/*----------------------------------------------------------------------------*/
  // for(;;)
   // {
      // Обработка передачи
      while(usartinTX != usartoutTX)
      {
         while (0 == __HAL_UART_GET_FLAG(pxDebug, UART_FLAG_TXE));
         pxDebug->Instance->DR = usartbuftx[usartoutTX];
         usartoutTX++;
         usartoutTX &= USART_TXBUFMASK;
      }
      //xSemaphoreTake(usarttxSemaphore, portMAX_DELAY);
   // }
}

static void usartputc(char c)
{
   usartbuftx[usartinTX++] = c;
   usartinTX &= USART_TXBUFMASK;
}
 
//Замена \n (LF 0A) на \r\n (CRLF 0D0A),
// \r во входной строке пропускается.
static void usartputs(const char *c)
{
   while (*c)
   {
      if (*c == '\n')
         usartputc('\r'); // 0x0D = insert carriage return
      usartputc(*c);
      c++;
   }
}
 
int uprintf (const char *fmt, ...)
{
   int16_t outsize;
   va_list ap;
 
   // Вернет ошибку, если формат является указателем NULL:
   if (!fmt) { return -1; }
   // Вернет ошибку, если строка превышает размер буфера, с учетом
   // необходимых дополнительных 2 символов: CR и нулевой терминатор ASCIIZ:
   if (UPRINTF_BUF_SIZE-2 < strlen(fmt)) { return -1; }
 
   va_start (ap,fmt);
   outsize = vsprintf(strbuf,fmt,ap);
   strbuf[outsize+1] = 0;
   va_end (ap);
 
   usartputs(strbuf);
   task_print(NULL);
   //xSemaphoreGive(usarttxSemaphore);
   return (int)outsize;
}

//#endif //_TEST_