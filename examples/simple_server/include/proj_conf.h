#ifndef _PROJ_CONF_SRV_H_
#define _PROJ_CONF_SRV_H_

#include "userint.h"

// Команда "это сервер" для препроцессора
#define SRV

#define   _TEST_    // для проверка на столе
//#define _TARGET_  // для работы в цехе

#define SERV_IS_407IC
//#define   SERV_IS_PI396

#define CLT_NUM_MAX                     (4U)
#define SRV_NUM_MAX                     (4U)
#define RMT_CLT_MAX                     (4U)

// Вывод отладочной инф-ции
// 1) на UART3 для платы 407IC, 2) в окно "Terminal" для PI396/PI396IZM1
// Во втором случае использовать define только в отладке, иначе ПО зависнет
//#define NET_DEBUG                       LWIP_DBG_OFF
//#define DBG_ON                          LWIP_DBG_ON // должен заменить NET_DEBUG

//#define DBG_TERMINAL                    1
//#define DBG_SHELL                       1

//#error "fuck "you"

// Сетевые настройки
// бит8 (multicast lолжен быть выставлен для Wireshark)
#define MAC_ADDR                        { 1U, 0U, 0U, 0U, 0U, 0x11U }
#define IP_ADDR                         "192.168.1.12"
#define MASK_ADDR                       "255.255.255.0"
#define GW_ADDR                         "192.168.1.1"


// адресация для PHY_MAX_LEN=2
// 1 - для платы 407IC, 0 - для платы 396
#if   (defined SERV_IS_407IC)
  #define  PHY_ADDR                     { 1U, 0U }
#elif (defined SERV_IS_PI396)
  #define  PHY_ADDR                     { 0U, 0U }
#endif

// 1 - для платы 407IC, 0 - для платы 396
// #if defined SERV_IS_407IC
  // #define  PHY_ADDR                     (1L) 
// #elif defined SERV_IS_PI396
  // #define  PHY_ADDR                     (0L)
// #endif

// Настрйоки UART
/* #if defined SERV_IS_407IC
  #define UART1_DE_PORT                 GPIOE
  #define UART1_DE_PIN                  GPIO_ODR_ODR_2
#elif defined SERV_IS_PI396
  #define UART1_DE_PORT                 GPIOA
  #define UART1_DE_PIN                  GPIO_ODR_ODR_8
#endif */

#endif //_PROJ_CONF_SRV_H_