#include "libiec61850/utils/SBuffer.h"
#include "string.h"

/**	----------------------------------------------------------------------------
	* @brief ??? */
void
	SBuffer_Init(SBuffer* sbuf, u8_t* buf, u16_t maxsize) {
/*----------------------------------------------------------------------------*/		
	sbuf->pcBuf = buf;
	sbuf->xSize.ulMax = maxsize;
	
	sbuf->xSize.ulFront = 0; 
	sbuf->xSize.ulSpace = sbuf->xSize.ulMax;
	memset( (sbuf->pcBuf), 0, sbuf->xSize.ulSpace*sizeof(u8_t) );
}
	
/**	----------------------------------------------------------------------------
	* @brief ??? */
u32_t
	SBuffer_GetPayloadSize(SBuffer* sbuf) {
/*----------------------------------------------------------------------------*/
	if (!sbuf) return 0;
	//if (sbuf->xSize.ulSpace > sbuf->xSize.ulMax)
	//	return 0;
	return (sbuf->xSize.ulMax - sbuf->xSize.ulSpace);
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
u8_t*
	SBuffer_GetPayload(SBuffer* sbuf) {
/*----------------------------------------------------------------------------*/
	if (!sbuf) return 0;
	//if (sbuf->xSize.ulSpace > sbuf->xSize.ulMax)
	//	return 0;
	return (sbuf->pcBuf + sbuf->xSize.ulSpace);
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
u8_t*
	SBuffer_GetFront(SBuffer* sbuf) {
/*----------------------------------------------------------------------------*/
	if (!sbuf) return 0;
	return (sbuf->pcBuf);
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
u32_t
	SBuffer_GetFrontSize(SBuffer* sbuf) {
/*----------------------------------------------------------------------------*/
	if (!sbuf) return 0;
	return (sbuf->xSize.ulFront);
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
void
	SBuffer_SetFrontSize(SBuffer* sbuf, u32_t size) {
/*----------------------------------------------------------------------------*/
	if (!sbuf) return;
	if (size > sbuf->xSize.ulMax) return;
	sbuf->xSize.ulFront = size;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
//#pragma inline=forced_no_body
void
	SBuffer_SetToFront(SBuffer* sbuf, u8_t value) {
/*----------------------------------------------------------------------------*/
	if (!sbuf) return;
	sbuf->pcBuf[sbuf->xSize.ulFront++] = value;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
//#pragma inline=forced_no_body
u8_t
	SBuffer_GetValue(SBuffer* sbuf, u32_t pos) {
/*----------------------------------------------------------------------------*/
	if (!sbuf) return (u8_t)0;
	if (pos > sbuf->xSize.ulMax) return (u8_t)0;
	return sbuf->pcBuf[sbuf->xSize.ulSpace + pos];
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
void
	SBuffer_Clear(SBuffer* sbuf) {
/*----------------------------------------------------------------------------*/
	// Pointers must be actual
	if (!sbuf | !sbuf->pcBuf) return;
	
	// Front to 0, Space to Max
	sbuf->xSize.ulSpace = sbuf->xSize.ulMax;
	sbuf->xSize.ulFront = 0;
	
	// setting space by zeros
	memset( (sbuf->pcBuf), 0, sbuf->xSize.ulSpace*sizeof(u8_t) );
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
u32_t
	SBuffer_Shift(SBuffer* sbuf) {
/*----------------------------------------------------------------------------*/
	// Указатели должны быть рабочими
	// Pointers must be actual
	if (!sbuf | !sbuf->pcBuf) return 0;
	
	// следующие полня не должны иметь нулевые значения 
	// Next filed must't have zero values
	if (!sbuf->xSize.ulMax | !sbuf->xSize.ulSpace | !sbuf->xSize.ulFront)
		return 0;
	
	// data moves to end
	memmove( (sbuf->pcBuf + sbuf->xSize.ulSpace - sbuf->xSize.ulFront), 
		(sbuf->pcBuf), sbuf->xSize.ulFront );
	
	// counter ulFront to 0, ulSpace reduces
	sbuf->xSize.ulSpace -= sbuf->xSize.ulFront;
	sbuf->xSize.ulFront = 0;
	
	// setting space by zeros
	memset( (sbuf->pcBuf), 0, sbuf->xSize.ulSpace*sizeof(u8_t) );
	
	return 1;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
void
	SBuffer_Replace(SBuffer* sbuf) {
/*----------------------------------------------------------------------------*/
	// присваивая Space значение Max создаем иллюзию, что массив пустой 
	sbuf->xSize.ulSpace = sbuf->xSize.ulMax;
	// Двигаем Front в Payload
	SBuffer_Shift(sbuf);
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
s32_t
	SBuffer_Writer(const void *buffer, u32_t size, void *app_key) {
/*----------------------------------------------------------------------------*/
	//локальные переменные
	u8_t* extbuf = (u8_t*)buffer;
	SBuffer* sbuf = (SBuffer*) app_key;
	
	if (sbuf->xSize.ulFront+size > sbuf->xSize.ulSpace)
		return 0;
	memcpy(sbuf->pcBuf + sbuf->xSize.ulFront, extbuf, size);
	sbuf->xSize.ulFront += size;
	return 1;
}

