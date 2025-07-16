#ifndef _SBUFFER_H_
#define _SBUFFER_H_

//#include "server/byte_buffer.h"
#include "userint.h"


	// Буффер
	// Max	 	 - Размер буффера
	// Front 	 - Длина временно используемой части
	// Space 	 - Пустое пространство, доступное под запись
	// Payload - Окомплетованные данные
	// 			 [-------|-------|---------]
	//        >Front< >Space< >Payload<
	//				>				  Max           <
typedef struct {
	u8_t* pcBuf;
	struct {
		u32_t ulMax;
		u32_t ulFront;
		u32_t ulSpace;
	} xSize;
} SBuffer;

typedef SBuffer * SBufferPtr;


void  SBuffer_Init(SBuffer* sbuf, u8_t* buf, u16_t maxsize);
u32_t	SBuffer_GetPayloadSize(SBuffer* sbuf);
u8_t *SBuffer_GetPayload(SBuffer* sbuf);
u8_t *SBuffer_GetFront(SBuffer* sbuf);
u32_t	SBuffer_GetFrontSize(SBuffer* sbuf);
void	SBuffer_SetFrontSize(SBuffer* sbuf, u32_t size);
void	SBuffer_SetToFront(SBuffer* sbuf, u8_t value);
u8_t  SBuffer_GetValue(SBuffer* sbuf, u32_t pos);
u32_t SBuffer_Shift(SBuffer * self);
void  SBuffer_Replace(SBuffer* sbuf);
void  SBuffer_Clear(SBuffer* sbuf);
s32_t SBuffer_Writer(const void *buffer, u32_t size, void *app_key);

#endif /*_SBUFFER_H_*/