/**
	******************************************************************************
  * @file    mms_value.c
  * @author  Michael Zillgith
  * @brief   This file is part of libIEC61850.
  *****************************************************************************/

#include "libiec61850/mms/mms_value.h"
#include <stdlib.h>
#include "libiec61850/utils/string_utilities.h"
#include <string.h>
//#include "lwip/opt.h"
//#include "hal.h"

extern void
	memcpyReverseByteOrder(uint8_t* dst, uint8_t* src, int size);


/*******************************************************************************
	Locally viewed (static) routines prototypes
*******************************************************************************/

static void
setVisibleStringValue(MmsValue* value, char* string);

static int
bitStringByteSize(MmsValue* value);

static void
updateStructuredComponent(MmsValue* self, MmsValue* update);


/*******************************************************************************
	Locally viewed variables
*******************************************************************************/

static MmsValue* lvalue = NULL;


/*******************************************************************************
	Globally viewed routines definitions
*******************************************************************************/

void
MmsValue_setArrayElement(MmsValue* array, int index, MmsValue* elementValue)
{
	if (array->type != MMS_ARRAY)
		return;

	if ((index < 0) || (index >= array->value.array.size))
		return;

	array->value.array.components[index] = elementValue;
}


void
	MmsValue_setDeletable(MmsValue* value) {
	value->deleteValue = 1;
}


int
MmsValue_isDeletable(MmsValue* value) {
	return value->deleteValue;
}


MmsValue*
MmsValue_getElement(MmsValue* array, int index)
{
	if (array->type != MMS_ARRAY)
		return NULL;

	if ((index < 0) || (index >= array->value.array.size))
		return NULL;

	return array->value.array.components[index];
}


MmsValue*
MmsValue_createEmtpyArray(int size)
{
	MmsValue* array = calloc(1, sizeof(MmsValue));

	array->type = MMS_ARRAY;
	array->value.array.size = size;
	array->value.array.components = calloc(size, sizeof(MmsValue*));

	int i;
	for (i = 0; i < size; i++) {
		array->value.array.components[i] = NULL;
	}

	return array;
}


//CHANGE _VER_0_3_2_
void
MmsValue_delete(MmsValue* value)
{
    switch (value->type) {
    case MMS_INTEGER:
        Asn1PrimitiveValue_destroy(value->value.integer);
        break;
    case MMS_UNSIGNED:
        Asn1PrimitiveValue_destroy(value->value.unsignedInteger);
        break;
    case MMS_FLOAT:
        free(value->value.floatingPoint.buf);
        break;
    case MMS_BIT_STRING:
        free(value->value.bitString.buf);
        break;
    case MMS_OCTET_STRING:
        free(value->value.octetString.buf);
        break;
    case MMS_VISIBLE_STRING:
        if (value->value.visibleString != NULL)
            free(value->value.visibleString);
        break;
    case MMS_STRING:
        if (value->value.mmsString != NULL)
            free(value->value.mmsString);
        break;
    case MMS_STRUCTURE:
        {
            int componentCount = value->value.structure.componentCount;
            int i;

            for (i = 0; i < componentCount; i++) {
                MmsValue_delete(value->value.structure.components[i]);
            }
        }
        free(value->value.structure.components);
        break;
    case MMS_ARRAY:
        {
            int size = value->value.array.size;
            int i;
            for (i = 0; i < size; i++) {
                MmsValue_delete(value->value.array.components[i]);
            }
        }
        free(value->value.array.components);
        break;
    }

	free(value);
}


MmsValue*
MmsValue_newDefaultValue(MmsTypeSpecification* typeSpec)
{
	MmsValue* value;
	
	lvalue = value;
	
//	LWIP_DEBUGF(ISO9506_DEBUG, ("MmsValue_newDefaultValue: ENTER\r\n"));

	switch (typeSpec->type) {
	case MMS_INTEGER:
		value = MmsValue_newInteger(typeSpec->typeSpec.integer);
		////LWIP_DEBUGF(ISO9506_DEBUG, ("MmsValue_newDefaultValue: MMS_INTEGER\r\n"));
		break;
	case MMS_UNSIGNED:
		value = MmsValue_newUnsigned(typeSpec->typeSpec.unsignedInteger);
		//LWIP_DEBUGF(ISO9506_DEBUG, ("MmsValue_newDefaultValue: MMS_UNSIGNED\r\n"));
		break;
	case MMS_FLOAT:
		value = calloc(1, sizeof(MmsValue));
		value->type = MMS_FLOAT;
		value->value.floatingPoint.exponentWidth = typeSpec->typeSpec.floatingpoint.exponentWidth;
		value->value.floatingPoint.formatWidth = typeSpec->typeSpec.floatingpoint.formatWidth;
		value->value.floatingPoint.buf = calloc(1, typeSpec->typeSpec.floatingpoint.formatWidth / 8);
		//LWIP_DEBUGF(ISO9506_DEBUG, ("MmsValue_newDefaultValue: MMS_FLOAT\r\n"));
		//value->value.floatingPoint.buf[2] = 1;
//		value->value.floatingPoint.buf[3] = 0x1;
//		value->value.floatingPoint.buf[4] = 0x0;
		break;
	case MMS_BIT_STRING:
		value = calloc(1, sizeof(MmsValue));
		value->type = MMS_BIT_STRING;
		{
			int bitSize = abs(typeSpec->typeSpec.bitString);
			value->value.bitString.size = bitSize;
			int size = (bitSize / 8) + ((bitSize % 8) > 0);
			value->value.bitString.buf = calloc(1, size);
		}
		//LWIP_DEBUGF(ISO9506_DEBUG, ("MmsValue_newDefaultValue: MMS_BIT_STRING\r\n"));
		break;
	case MMS_OCTET_STRING:
		value = calloc(1, sizeof(MmsValue));
		value->type = MMS_OCTET_STRING;

		if (typeSpec->typeSpec.octetString < 0)
			value->value.octetString.size = 0;
		else
			value->value.octetString.size = typeSpec->typeSpec.octetString;

		value->value.octetString.maxSize = abs(typeSpec->typeSpec.octetString);
		value->value.octetString.buf = calloc(1, abs(typeSpec->typeSpec.octetString));
		break;
	case MMS_VISIBLE_STRING:
		value = MmsValue_newVisibleString(NULL);
		//LWIP_DEBUGF(ISO9506_DEBUG, ("MmsValue_newDefaultValue: MMS_VISIBLE_STRING\r\n"));
		break;
	case MMS_BOOLEAN:
		value = MmsValue_newBoolean(false);
		//LWIP_DEBUGF(ISO9506_DEBUG, ("MmsValue_newDefaultValue: MMS_BOOLEAN\r\n"));
		break;
	case MMS_UTC_TIME:
		value = calloc(1, sizeof(MmsValue));
		value->type = MMS_UTC_TIME;
		//LWIP_DEBUGF(ISO9506_DEBUG, ("MmsValue_newDefaultValue: MMS_UTC_TIME\r\n"));
		break;
	case MMS_ARRAY:
		value = MmsValue_createArray(typeSpec->typeSpec.array.elementTypeSpec,
				typeSpec->typeSpec.array.elementCount);
		//LWIP_DEBUGF(ISO9506_DEBUG, ("MmsValue_newDefaultValue: MMS_ARRAY\r\n"));
		break;
	case MMS_STRUCTURE:
		value = MmsValue_newStructure(typeSpec);
//		LWIP_DEBUGF(ISO9506_DEBUG, ("MmsValue_newDefaultValue: MMS_STRUCTURE\r\n"));
		break;
	case MMS_STRING:
		value = MmsValue_newMmsString(NULL);
		break;
	case MMS_BINARY_TIME:
		if (typeSpec->typeSpec.binaryTime == 4)
			value = MmsValue_newBinaryTime(true);
		else
			value = MmsValue_newBinaryTime(false);
		break;
	default:
		//LWIP_DEBUGF(ISO9506_DEBUG, ("MmsValue_newDefaultValue: UNK TYPE\r\n"));
		break;
	}
	
	value->deleteValue = 0;
	return value;
}


//CHANGE _VER_0_3_2_
MmsValue*
MmsValue_newBinaryTime(bool timeOfDay)
{
	MmsValue* value = calloc(1, sizeof(MmsValue));
	value->type = MMS_BINARY_TIME;

	if (timeOfDay == true)
		value->value.binaryTime.size = 4;
	else
		value->value.binaryTime.size = 6;

	return value;
}

static void
setMmsStringValue(MmsValue* value, char* string)
{
	if (string != NULL)
		value->value.mmsString = copyString(string);
	else
		value->value.mmsString = NULL;
}

MmsValue*
MmsValue_newMmsString(char* string)
{
	MmsValue* value = calloc(1, sizeof(MmsValue));
	value->type = MMS_VISIBLE_STRING;

	setMmsStringValue(value, string);

	return value;
}

void
MmsValue_setMmsString(MmsValue* value, char* string)
{
	if (value->type == MMS_STRING) {
		if (value->value.mmsString != NULL)
			free(value->value.mmsString);

		setMmsStringValue(value, string);
	}
}


MmsValue*
MmsValue_newInteger(int size /*integer size in bits*/)
{
	MmsValue* value = calloc(1, sizeof(MmsValue));
	value->type = MMS_INTEGER;

	if (size <= 32)
		value->value.integer = BerInteger_createInt32();
	else
		value->value.integer = BerInteger_createInt64();

	return value;
}


MmsValue*
MmsValue_newUnsigned(int size /*integer size in bits*/)
{
	MmsValue* value = calloc(1, sizeof(MmsValue));
	value->type = MMS_UNSIGNED;
	
	lvalue = value;

	if (size <= 32)
		value->value.unsignedInteger = BerInteger_createInt32();
	else
		value->value.unsignedInteger = BerInteger_createInt64();
	

	return value;
}


MmsValue*
MmsValue_newVisibleString(char* string)
{
	MmsValue* value = calloc(1, sizeof(MmsValue));
	value->type = MMS_VISIBLE_STRING;

	setVisibleStringValue(value, string);

	return value;
}


//CHANGE _VER_0_3_3_
MmsValue*
MmsValue_newBoolean(bool boolean)
{
	MmsValue* self = calloc(1, sizeof(MmsValue));
	self->type = MMS_BOOLEAN;
	if (boolean == true)
		self->value.boolean = 1;
	else
		self->value.boolean = 0;

	return self;
}


MmsValue*
MmsValue_createArray(MmsTypeSpecification* elementType, int size)
{
	MmsValue* array = calloc(1, sizeof(MmsValue));

	array->type = MMS_ARRAY;
	array->value.array.size = size;
	array->value.array.components = calloc(size, sizeof(MmsValue*));

	int i;
	for (i = 0; i < size; i++) {
		array->value.array.components[i] = MmsValue_newDefaultValue(elementType);
	}

	return array;
}


//CHANGE _VER_0_3_3_
MmsValue*
MmsValue_newOctetString(int size, int maxSize)
{
	MmsValue* self = calloc(1, sizeof(MmsValue));
	self->type = MMS_OCTET_STRING;
	self->value.octetString.size = size;
	self->value.octetString.maxSize = maxSize;
	self->value.octetString.buf = calloc(1, maxSize);

	return self;
}


//CHANGE _VER_0_3_3_
MmsValue*
MmsValue_newStructure(MmsTypeSpecification* typeSpec)
{
	MmsValue* self = calloc(1, sizeof(MmsValue));

	self->type = MMS_STRUCTURE;
	int componentCount = typeSpec->typeSpec.structure.elementCount;
	self->value.structure.componentCount = componentCount;
	self->value.structure.components = calloc(componentCount, sizeof(MmsValue*));

	int i;
	for (i = 0; i < componentCount; i++) {
		self->value.structure.components[i] =
				MmsValue_newDefaultValue(typeSpec->typeSpec.structure.elements[i]);
	}

	return self;
}


MmsValue*
MmsValue_newIntegerFromBerInteger(Asn1PrimitiveValue* berInteger)
{
	MmsValue* self = calloc(1, sizeof(MmsValue));
	self->type = MMS_INTEGER;

	self->value.integer = berInteger;

	return self;
}


MmsValue*
MmsValue_newUnsignedFromBerInteger(Asn1PrimitiveValue* berInteger)
{
	MmsValue* self = calloc(1, sizeof(MmsValue));
	self->type = MMS_UNSIGNED;

	self->value.unsignedInteger = berInteger;

	return self;
}


MmsValue*
MmsValue_newVisibleStringFromByteArray(uint8_t* byteArray, int size)
{
	MmsValue* value = calloc(1, sizeof(MmsValue));
	value->type = MMS_VISIBLE_STRING;

	value->value.visibleString = createStringFromBuffer(byteArray, size);

	return value;
}

MmsValue*
MmsValue_newIntegerFromInt32(int32_t integer)
{
	MmsValue* value = malloc(sizeof(MmsValue));

	value->type = MMS_INTEGER;
	value->value.integer = BerInteger_createFromInt32(integer);

	return value;
}

MmsValue*
MmsValue_newUnsignedFromUint32(uint32_t integer)
{
	MmsValue* value = malloc(sizeof(MmsValue));

	value->type = MMS_UNSIGNED;
	value->value.unsignedInteger = BerInteger_createFromUint32(integer);

	return value;
}


int32_t
MmsValue_toInt32(MmsValue* value)
{
	int32_t integerValue = 0;

	if (value->type == MMS_INTEGER)
		BerInteger_toInt32(value->value.integer, &integerValue);
	else if (value->type == MMS_UNSIGNED)
		BerInteger_toInt32(value->value.unsignedInteger, &integerValue);

	return integerValue;
}

uint32_t
MmsValue_toUint32(MmsValue* value)
{
	uint32_t integerValue = 0;

	if (value->type == MMS_INTEGER)
		BerInteger_toUint32(value->value.integer, &integerValue);
	else if (value->type == MMS_UNSIGNED)
		BerInteger_toUint32(value->value.unsignedInteger, &integerValue);

	return integerValue;
}


//CHANGE _VER_0_3_2_
// create a deep clone
MmsValue*
MmsValue_clone(MmsValue* value)
{
	MmsValue* newValue = calloc(1, sizeof(MmsValue));
	newValue->deleteValue = value->deleteValue;
	newValue->type = value->type;
	int size;

	switch(value->type) {
	//TODO implement cases for remaining types: ...
	case MMS_ARRAY:
		{
			int componentCount = value->value.array.size;
			newValue->value.array.size = componentCount;
			newValue->value.array.components = calloc(componentCount, sizeof(MmsValue*));
			int i;
			for (i = 0; i < componentCount; i++) {
				newValue->value.array.components[i] =
						MmsValue_clone(value->value.array.components[i]);
			}
		}
		break;
	case MMS_STRUCTURE:
		{
			int componentCount = value->value.structure.componentCount;
			newValue->value.structure.componentCount = componentCount;
			newValue->value.structure.components = calloc(componentCount, sizeof(MmsValue*));
			int i;
			for (i = 0; i < componentCount; i++) {
				newValue->value.structure.components[i] =
						MmsValue_clone(value->value.structure.components[i]);
			}
		}
		break;
	case MMS_INTEGER:
		newValue->value.integer = Asn1PrimitiveValue_clone(value->value.integer);
		break;
	case MMS_UNSIGNED:
		newValue->value.unsignedInteger = Asn1PrimitiveValue_clone(value->value.unsignedInteger);
		break;
	case MMS_FLOAT:
		newValue->value.floatingPoint.formatWidth = value->value.floatingPoint.formatWidth;
		newValue->value.floatingPoint.exponentWidth = value->value.floatingPoint.exponentWidth;
		size = value->value.floatingPoint.formatWidth / 8;
		newValue->value.floatingPoint.buf = malloc(size);
		memcpy(newValue->value.floatingPoint.buf, value->value.floatingPoint.buf, size);
		break;
	case MMS_BIT_STRING:
		newValue->value.bitString.size = value->value.bitString.size;
		size = bitStringByteSize(value);
		newValue->value.bitString.buf = malloc(size);
		memcpy(newValue->value.bitString.buf, value->value.bitString.buf, size);
		break;
	case MMS_BOOLEAN:
		newValue->value.boolean = value->value.boolean;
		break;
	case MMS_OCTET_STRING:
		size = value->value.octetString.size;
		newValue->value.octetString.size = size;
		newValue->value.octetString.maxSize  = value->value.octetString.maxSize;
		newValue->value.octetString.buf = malloc(value->value.octetString.maxSize);
		memcpy(newValue->value.octetString.buf, value->value.octetString.buf, size);
		break;
	case MMS_VISIBLE_STRING:
		size = strlen(value->value.visibleString) + 1;
		newValue->value.visibleString = malloc(size);
		strcpy(newValue->value.visibleString, value->value.visibleString);
		break;
	case MMS_UTC_TIME:
		memcpy(newValue->value.utcTime, value->value.utcTime, 8);
		break;
	case MMS_BINARY_TIME:
	    newValue->value.binaryTime.size = value->value.binaryTime.size;
	    memcpy(newValue->value.binaryTime.buf, value->value.binaryTime.buf, 6);
	    break;
	case MMS_STRING:
	    size = strlen(value->value.mmsString) + 1;
        newValue->value.mmsString = malloc(size);
        strcpy(newValue->value.mmsString, value->value.mmsString);
	    break;
	}

	return newValue;
}


MmsValue*
MmsValue_getStructElementByIndex(MmsValue* structure, int index)
{
	if (structure->type != MMS_STRUCTURE)
		return NULL;

	if ((index < 0) || (index >= structure->value.structure.componentCount))
		return NULL;

	return structure->value.structure.components[index];
}


MmsValue*
MmsValue_newFloat(float variable)
{
	MmsValue* value = malloc(sizeof(MmsValue));

	value->type = MMS_FLOAT;
	value->value.floatingPoint.formatWidth = 32;
	value->value.floatingPoint.exponentWidth = 8;
	value->value.floatingPoint.buf = malloc(4);

	*((float*) value->value.floatingPoint.buf) = variable;

	return value;
}


BOOL
	MmsValue_setFloat(MmsValue* value, float newFloatValue) {

	if (value == NULL)
		return FALSE;
	
	if (value->type == MMS_FLOAT) {
		if (value->value.floatingPoint.formatWidth == 32) {
			*((float*) value->value.floatingPoint.buf) = newFloatValue;
		}
		else if (value->value.floatingPoint.formatWidth == 64) {
			*((double*) value->value.floatingPoint.buf) = (double) newFloatValue;
		} else {
			return FALSE;
		}
	} else {
		return FALSE;
	}

	return TRUE;
}


float
	MmsValue_toFloat(MmsValue* value) {

	if (value == NULL)
		return 0.f;

	if (value->type == MMS_FLOAT) {
		if (value->value.floatingPoint.formatWidth == 32) {
			float val;

			val = *((float*) (value->value.floatingPoint.buf));
			return val;
		}
		else if (value->value.floatingPoint.formatWidth == 64) {
			float val;
			val = *((double*) (value->value.floatingPoint.buf));
			return val;
		}
	}
	//else
	//	printf("MmsValue_toFloat: conversion error. Wrong type!\r\n");

	return 0.f;
}


bool
MmsValue_update(MmsValue* self, MmsValue* update) {
	
	int retval;
	
	if (self->type == update->type) {
		switch (self->type) {
		case MMS_STRUCTURE:
		case MMS_ARRAY:
			updateStructuredComponent(self, update);
			break;
		case MMS_BOOLEAN:
			self->value.boolean = update->value.boolean;
			break;
		case MMS_FLOAT:
			if (self->value.floatingPoint.formatWidth == update->value.floatingPoint.formatWidth) {
				self->value.floatingPoint.exponentWidth = update->value.floatingPoint.exponentWidth;
				memcpy(self->value.floatingPoint.buf, update->value.floatingPoint.buf,
						self->value.floatingPoint.formatWidth / 8);
			}
			else return false;
			break;
		case MMS_INTEGER:
			if (BerInteger_setFromBerInteger(self->value.integer, update->value.integer))
				return true;
			else
				return false;
			break;
		case MMS_UNSIGNED:
			lvalue = update;
			retval = BerInteger_setFromBerInteger(self->value.unsignedInteger,
					update->value.unsignedInteger);
			if (retval)
				return true;
			else
				return false;
			break;
		case MMS_UTC_TIME:
			memcpy(self->value.utcTime, update->value.utcTime, 8);
			break;
		case MMS_BIT_STRING:
			if (self->value.bitString.size == update->value.bitString.size)
				memcpy(self->value.bitString.buf, update->value.bitString.buf, bitStringByteSize(self));
			else return false;
			break;
		case MMS_OCTET_STRING:
			if (self->value.octetString.maxSize == update->value.octetString.maxSize) {
				memcpy(self->value.octetString.buf, update->value.octetString.buf,
						update->value.octetString.size);

				self->value.octetString.size = update->value.octetString.size;
			}
			else return false;
			break;
		case MMS_VISIBLE_STRING:
			MmsValue_setVisibleString(self, update->value.visibleString);
			break;
		case MMS_STRING:
			MmsValue_setMmsString(self, update->value.mmsString);
			break;
		case MMS_BINARY_TIME:
			self->value.binaryTime.size = update->value.binaryTime.size;
			memcpy(self->value.binaryTime.buf, update->value.binaryTime.buf,
					update->value.binaryTime.size);
			break;
		default:
			return false;
			break;
		}
		return true;
	}
	else
		return false;
}

//CHANGE _V_0_3_0_
static int
getBitStringByteSize(MmsValue* self)
{
	int byteSize;

	if (self->value.bitString.size % 8)
		byteSize = (self->value.bitString.size / 8) + 1;
	else
		byteSize = self->value.bitString.size / 8;

	return byteSize;
}

//CHANGE _V_0_3_0_
void
MmsValue_deleteAllBitStringBits(MmsValue* self)
{
	int byteSize = getBitStringByteSize(self);

	int i;
	for (i = 0; i < byteSize; i++) {
		self->value.bitString.buf[i] = 0;
	}
}

//CHANGE _V_0_3_0_
void
MmsValue_setAllBitStringBits(MmsValue* self)
{
	int byteSize = getBitStringByteSize(self);

	int i;
	for (i = 0; i < byteSize; i++) {
		self->value.bitString.buf[i] = 0xff;
	}
}

void
MmsValue_setBitStringBit(MmsValue* self, int bitPos, bool value)
{
	if (bitPos < self->value.bitString.size) {
		int bytePos = bitPos / 8;
		// int bitPosInByte = bitPos % 8;
		int bitPosInByte = 7 - (bitPos % 8);

		int bitMask = (1 << bitPosInByte);

		if (value)
			self->value.bitString.buf[bytePos] |= bitMask;
		else
			self->value.bitString.buf[bytePos] &= (~bitMask);
	}
}

bool
MmsValue_getBitStringBit(MmsValue* self, int bitPos)
{
	if (bitPos < self->value.bitString.size) {
		int bytePos = bitPos / 8;
		//int bitPosInByte = bitPos % 8;

		int bitPosInByte = 7 - (bitPos % 8);

		int bitMask = (1 << bitPosInByte);

		if ((self->value.bitString.buf[bytePos] & bitMask) > 0)
			return true;
		else
			return false;

	}
	else return false; /* out of range bits are always zero */
}


void
MmsValue_setVisibleString(MmsValue* value, char* string)
{
	if (value->type == MMS_VISIBLE_STRING) {
		if (value->value.visibleString != NULL)
			free(value->value.visibleString);

		setVisibleStringValue(value, string);
	}
}

void
MmsValue_setUint16(MmsValue* value, uint16_t integer)
{
	if (value->type == MMS_UNSIGNED) {
		if (Asn1PrimitiveValue_getMaxSize(value->value.integer) >= 2) {
				BerInteger_setUint16(value->value.integer, integer);
		}
	}
}

void
MmsValue_setInt32(MmsValue* value, int32_t integer)
{
	if (value->type == MMS_INTEGER) {
		if (Asn1PrimitiveValue_getMaxSize(value->value.integer) == 4) {
				BerInteger_setInt32(value->value.integer, integer);
		}
	}
}

void
MmsValue_setBoolean(MmsValue* value, bool boolValue)
{
	value->value.boolean = boolValue;
}


MmsValue*
MmsValue_setUtcTime(MmsValue* value, uint32_t timeval)
{
	uint8_t* timeArray = (uint8_t*) &timeval;
	uint8_t* valueArray = value->value.utcTime;

#ifdef ORDER_LITTLE_ENDIAN
		memcpyReverseByteOrder(valueArray, timeArray, 4);
#else
		memcpy(valueArray, timeArray, 4);
#endif

	return value;
}


//CHANGE _V_0_3_1_
MmsValue*
MmsValue_newDataAccessError(uint32_t code){
    MmsValue* self = calloc(1, sizeof(MmsValue));

    self->type = MMS_DATA_ACCESS_ERROR;
    self->value.dataAccessError.code = code;

    return self;
}


//CHANGE _VER_0_3_2_
MmsValue*
	MmsValue_setUtcTimeMs(MmsValue* value, uint32_t * timestamp) {
	
	//u32_t sntp_time_sec = 0, sntp_time_us = 0;
	//u32_t timestamp[2] = {0,0};
	
	//GET_SYSTEM_TIME((timestamp+0),(timestamp+1));

  uint8_t* timeArray = (uint8_t*) (timestamp+0);
	uint8_t* valueArray = value->value.utcTime;

#ifdef ORDER_LITTLE_ENDIAN
		memcpyReverseByteOrder(valueArray, timeArray, 4);
#else
		memcpy(valueArray, timeArray, 4);
#endif

	uint32_t remainder = (timestamp[1] / 1000L);
	uint32_t fractionOfSecond = (remainder) * 16777 + ((remainder * 216) / 1000);
	
	/* encode fraction of second */
	valueArray[4] = ((fractionOfSecond >> 16) & 0xff);
	valueArray[5] = ((fractionOfSecond >> 8) & 0xff);
	valueArray[6] = (fractionOfSecond & 0xff);

	/* encode time quality */
	valueArray[7] = 0x0a; /* 10 bit sub-second time accuracy */

	return value;
}


//CHANGE _VER_0_3_2_
uint64_t
MmsValue_getUtcTimeInMs(MmsValue* value)
{
    uint32_t timeval32;
    uint8_t* valueArray = value->value.utcTime;

#ifdef ORDER_LITTLE_ENDIAN
    memcpyReverseByteOrder((uint8_t*)&timeval32, valueArray, 4);
#else
    memcpy(&timeval32, valueArray, 4);
#endif

    uint32_t fractionOfSecond = 0;

    fractionOfSecond = (valueArray[4] << 16);
    fractionOfSecond += (valueArray[5] << 8);
    fractionOfSecond += (valueArray[6]);

    uint32_t remainder = fractionOfSecond / 16777;

    uint64_t msVal = (timeval32 * 1000LL) + remainder;

    return (uint64_t) msVal;
}


//CHANGE _VER_0_3_2_
uint32_t
MmsValue_toUnixTimestamp(MmsValue* value)
{
	uint32_t timestamp;
	uint8_t* timeArray = (uint8_t*) &timestamp;

#ifdef ORDER_LITTLE_ENDIAN
		timeArray[0] = value->value.utcTime[3];
		timeArray[1] = value->value.utcTime[2];
		timeArray[2] = value->value.utcTime[1];
		timeArray[3] = value->value.utcTime[0];
#else
		timeArray[0] = value->value.utcTime[0];
    timeArray[1] = value->value.utcTime[1];
    timeArray[2] = value->value.utcTime[2];
    timeArray[3] = value->value.utcTime[3];
#endif	

	return timestamp;
}


//CHANGE _VER_0_3_2_
void
MmsValue_setBinaryTime(MmsValue* value, uint64_t timestamp)
{
    uint64_t mmsTime = timestamp - (441763200000LL);

    uint8_t* binaryTimeBuf = value->value.binaryTime.buf;

    if (value->value.binaryTime.size == 6) {
        uint16_t daysDiff = mmsTime / (86400000LL);
        uint8_t* daysDiffBuf = (uint8_t*)&daysDiff;

#ifdef ORDER_LITTLE_ENDIAN
				binaryTimeBuf[4] = daysDiffBuf[1];
        binaryTimeBuf[5] = daysDiffBuf[0];
#else
				binaryTimeBuf[4] = daysDiffBuf[0];
        binaryTimeBuf[5] = daysDiffBuf[1];
#endif
    }

    uint32_t msSinceMidnight = mmsTime % (86400000LL);
    uint8_t* msSinceMidnightBuf = (uint8_t*)&msSinceMidnight;

#ifdef ORDER_LITTLE_ENDIAN
				binaryTimeBuf[0] = msSinceMidnightBuf[3];
        binaryTimeBuf[1] = msSinceMidnightBuf[2];
        binaryTimeBuf[2] = msSinceMidnightBuf[1];
        binaryTimeBuf[3] = msSinceMidnightBuf[0];
#else
				binaryTimeBuf[0] = msSinceMidnightBuf[0];
        binaryTimeBuf[1] = msSinceMidnightBuf[1];
        binaryTimeBuf[2] = msSinceMidnightBuf[2];
        binaryTimeBuf[3] = msSinceMidnightBuf[3];
#endif
}


//CHANGE _VER_0_3_2_
uint64_t
MmsValue_getBinaryTimeAsUtcMs(MmsValue* value)
{
    uint64_t timestamp = 0;

    uint8_t* binaryTimeBuf = value->value.binaryTime.buf;

    if (value->value.binaryTime.size == 6) {

        uint16_t daysDiff;

        daysDiff = binaryTimeBuf[4] * 256;
        daysDiff += binaryTimeBuf[5];

        uint64_t mmsTime;

        mmsTime = daysDiff * (86400000LL);


        timestamp = mmsTime + (441763200000LL);
    }

    uint32_t msSinceMidnight = 0;

    msSinceMidnight = binaryTimeBuf[0] << 24;
    msSinceMidnight += binaryTimeBuf[1] << 16;
    msSinceMidnight += binaryTimeBuf[2] << 8;
    msSinceMidnight += binaryTimeBuf[3];

    timestamp += msSinceMidnight;

    return timestamp;
}


//CHANGE _VER_0_3_2_
char*
MmsValue_toString(MmsValue* value)
{
	if (value->type == MMS_VISIBLE_STRING)
		return value->value.visibleString;
	else if (value->type == MMS_STRING)
	    return value->value.mmsString;

	return NULL;
}


//CHANGE _VER_0_3_2_

MmsValue*
MmsValue_newUtcTimeByTimestamp(uint32_t * timestamp)
{
    MmsValue* value = calloc(1, sizeof(MmsValue));
    value->type = MMS_UTC_TIME;

    MmsValue_setUtcTimeMs(value, timestamp);

    return value;
}


//CHANGE _VER_0_3_3_
bool
MmsValue_isEqual(MmsValue* self, MmsValue* otherValue)
{
    if (self->type == otherValue->type) {
        switch (self->type) {
        case MMS_STRUCTURE:
            if (self->value.structure.componentCount == otherValue->value.structure.componentCount) {
                int componentCount = self->value.structure.componentCount;

                int i;
                for (i = 0; i < componentCount; i++) {
                    if (!MmsValue_isEqual(self->value.structure.components[i],
                            otherValue->value.structure.components[i]))
                        return false;
                }

                return true;
            }
            break;

        case MMS_ARRAY:

            if (self->value.array.size == otherValue->value.array.size) {
                int componentCount = self->value.array.size;

                int i;
                for (i = 0; i < componentCount; i++) {
                    if (MmsValue_isEqual(self->value.array.components[i],
                            otherValue->value.array.components[i]) == false)
                        return false;
                }

                return true;
            }
            break;
        case MMS_BOOLEAN:
            if (self->value.boolean == otherValue->value.boolean)
                return true;
            break;
        case MMS_FLOAT:
            if (memcmp(self->value.floatingPoint.buf, otherValue->value.floatingPoint.buf,
                    self->value.floatingPoint.formatWidth / 8) == 0)
                return true;
            break;
        case MMS_INTEGER:
            return Asn1PrimitivaValue_compare(self->value.integer, otherValue->value.integer);
            break;
        case MMS_UNSIGNED:
            return Asn1PrimitivaValue_compare(self->value.unsignedInteger, otherValue->value.unsignedInteger);
            break;
        case MMS_UTC_TIME:
            if (memcmp(self->value.utcTime, otherValue->value.utcTime, 8) == 0)
                return true;
            break;
        case MMS_BIT_STRING:
            if (self->value.bitString.size == otherValue->value.bitString.size) {
                if (memcmp(self->value.bitString.buf, otherValue->value.bitString.buf,
                        bitStringByteSize(self)) == 0)
                    return true;

            }
            break;
        case MMS_BINARY_TIME:
            if (self->value.binaryTime.size == otherValue->value.binaryTime.size) {
                if (memcmp(self->value.binaryTime.buf, otherValue->value.binaryTime.buf,
                        self->value.binaryTime.size) == 0)
                    return true;
            }
            break;

        case MMS_OCTET_STRING:
            if (self->value.octetString.size == otherValue->value.octetString.size) {
                if (memcmp(self->value.octetString.buf, otherValue->value.octetString.buf,
                        self->value.octetString.size) == 0)
                    return true;
            }
            break;

        case MMS_VISIBLE_STRING:
            if (self->value.visibleString != NULL) {
                if (otherValue->value.visibleString != NULL) {
                    if (strcmp(self->value.visibleString, otherValue->value.visibleString) == 0)
                        return true;
                }
            }
            else {
                if (otherValue->value.visibleString == NULL)
                    return true;
            }
            break;

        case MMS_STRING:
            if (self->value.mmsString != NULL) {
                if (otherValue->value.mmsString != NULL) {
                    if (strcmp(self->value.mmsString, otherValue->value.mmsString) == 0)
                        return true;
                }
            }
            else {
                if (otherValue->value.mmsString == NULL)
                    return true;
            }
            break;
        }
        return false;
    }
    else
        return false;
}


//CHANGE _VER_0_3_3_
void
MmsValue_setUint8(MmsValue* value, uint8_t integer)
{
    if (value->type == MMS_UNSIGNED) {
        if (Asn1PrimitiveValue_getMaxSize(value->value.integer) >= 1) {
            BerInteger_setUint8(value->value.integer, integer);
        }
    }

}


//CHANGE _VER_0_3_3_
bool
MmsValue_getBoolean(MmsValue* value)
{
    return value->value.boolean;
}


//CHANGE _VER_0_3_3_
void
MmsValue_setOctetString(MmsValue* self, uint8_t* buf, int size)
{
    if (size <= self->value.octetString.maxSize) {
        memcpy(self->value.octetString.buf, buf, size);
        self->value.octetString.size = size;
    }
}


MmsValue*
MmsValue_newUtcTime(uint32_t timeval)
{
	MmsValue* value = calloc(1, sizeof(MmsValue));
	value->type = MMS_UTC_TIME;

	uint8_t* timeArray = (uint8_t*) &timeval;
	uint8_t* valueArray = value->value.utcTime;

#ifdef ORDER_LITTLE_ENDIAN
	valueArray[0] = timeArray[3];
	valueArray[1] = timeArray[2];
	valueArray[2] = timeArray[1];
	valueArray[3] = timeArray[0];
#else
	valueArray[0] = timeArray[0];
	valueArray[1] = timeArray[1];
	valueArray[2] = timeArray[2];
	valueArray[3] = timeArray[3];
#endif

	return value;
}


MmsValue*
MmsValue_newBitString(int bitSize)
{
	MmsValue* self = malloc(sizeof(MmsValue));

	self->type = MMS_BIT_STRING;
	self->value.bitString.size = bitSize;
	self->value.bitString.buf = calloc(bitStringByteSize(self), 1);

	return self;
}


MmsType
MmsValue_getType(MmsValue* self)
{
	return self->type;
}


/*******************************************************************************
	Locally viewed (static) routines definitions
*******************************************************************************/

static void
updateStructuredComponent(MmsValue* self, MmsValue* update)
{
	int componentCount;
	MmsValue** selfValues;
	MmsValue** updateValues;

	if (self->type == MMS_STRUCTURE) {
		componentCount = self->value.structure.componentCount;
		selfValues = self->value.structure.components;
		updateValues = update->value.structure.components;
	}
	else {
		componentCount = self->value.array.size;
		selfValues = self->value.array.components;
		updateValues = update->value.array.components;
	}

	int i;
	for (i = 0; i < componentCount; i++) {
		MmsValue_update(selfValues[i], updateValues[i]);
	}
}


static void
setVisibleStringValue(MmsValue* value, char* string)
{
	if (string != NULL)
		value->value.visibleString = copyString(string);
	else
		value->value.visibleString = NULL;
}


static int
bitStringByteSize(MmsValue* value)
{
	int bitSize = value->value.bitString.size;
	return (bitSize / 8) + ((bitSize % 8) > 0);
}




