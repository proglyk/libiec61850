#ifndef _USERINT_H_
#define _USERINT_H_

#include "stdint.h"
#include "stm32f4xx.h"

// char 1 byte
typedef int8_t s8_t;
typedef uint8_t u8_t;

// short 2 bytes
typedef int16_t s16_t;
typedef uint16_t u16_t;

// long 4 bytes
typedef int32_t s32_t;
typedef uint32_t u32_t;

typedef float f32_t;

#define USLED4_ON() do  {GPIOI->ODR |= (1<<10);} while(0)
#define USLED4_OFF() do {GPIOI->ODR &= ~(1<<10);} while(0)
#define USLED4_TOG() do {GPIOI->ODR ^= (1<<10);} while(0)
  
#define USLED3_ON() do  {GPIOI->ODR |= (1<<8);} while(0)
#define USLED3_OFF() do {GPIOI->ODR &= ~(1<<8);} while(0)
#define USLED3_TOG() do {GPIOI->ODR ^= (1<<8);} while(0)
  
#define USLED2_ON() do  {GPIOH->ODR |= (1<<3);} while(0)
#define USLED2_OFF() do {GPIOH->ODR &= ~(1<<3);} while(0)
#define USLED2_TOG() do {GPIOH->ODR ^= (1<<3);} while(0)
  
#define USLED1_ON() do  {GPIOH->ODR |= (1<<2);} while(0)
#define USLED1_OFF() do {GPIOH->ODR &= ~(1<<2);} while(0)
#define USLED1_TOG() do {GPIOH->ODR ^= (1<<2);} while(0)


#endif /*_USERINT_H_*/