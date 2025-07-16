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


void  SBuffer_Init(SBuffer *, u8_t *, u16_t);
u32_t	SBuffer_GetPayloadSize(SBuffer *);
u8_t *SBuffer_GetPayload(SBuffer *);
u8_t *SBuffer_GetFront(SBuffer *);
u32_t	SBuffer_GetFrontSize(SBuffer *);
void	SBuffer_SetFrontSize(SBuffer *, u32_t);
void	SBuffer_SetToFront(SBuffer *, u8_t);
u8_t  SBuffer_GetValue(SBuffer *, u32_t);
u32_t SBuffer_Shift(SBuffer *);
void  SBuffer_Replace(SBuffer *);
void  SBuffer_Clear(SBuffer *);
s32_t SBuffer_Writer(const void *, u32_t, void *);

#endif /*_SBUFFER_H_*/