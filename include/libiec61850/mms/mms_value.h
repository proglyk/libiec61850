/**
	******************************************************************************
  * @file    mms_value.h
  * @author  Michael Zillgith
  * @brief   This file is part of libIEC61850.
  *****************************************************************************/

#ifndef __MMS_VALUE_H__
#define __MMS_VALUE_H__

#include "libiec61850/mms/mms_types.h"
#include <stdbool.h>
#include "temptypes.h"


/*******************************************************************************
	Globally viewed routines prototypes
*******************************************************************************/

void
MmsValue_setArrayElement(MmsValue* array, int index, MmsValue* elementValue);

void
MmsValue_delete(MmsValue* self);

MmsValue*
MmsValue_newDefaultValue(MmsTypeSpecification* typeSpec);

MmsValue*
MmsValue_newInteger(int size);

MmsValue*
MmsValue_newUnsigned(int size);

MmsValue*
MmsValue_newVisibleString(char* string);

MmsValue*
MmsValue_newBoolean(bool boolean);

MmsValue*
MmsValue_createArray(MmsTypeSpecification* elementType, int size);

MmsValue*
MmsValue_newStructure(MmsTypeSpecification* typeSpec);

MmsValue*
MmsValue_newIntegerFromBerInteger(Asn1PrimitiveValue* berInteger);

MmsValue*
MmsValue_newUnsignedFromBerInteger(Asn1PrimitiveValue* berInteger);

MmsValue*
	MmsValue_newVisibleStringFromByteArray(uint8_t* byteArray, int size);

int32_t
	MmsValue_toInt32(MmsValue* value);

MmsValue*
	MmsValue_getElement(MmsValue* array, int index);

MmsValue*
	MmsValue_createEmtpyArray(int size);

void
	MmsValue_setDeletable(MmsValue* self);
	
int
	MmsValue_isDeletable(MmsValue* self);

MmsValue*
MmsValue_clone(MmsValue* self);

MmsValue*
	MmsValue_getStructElementByIndex(MmsValue*, int);
	
MmsValue*
MmsValue_newFloat(float variable);

BOOL
	MmsValue_setFloat(MmsValue*, float);

float
	MmsValue_toFloat(MmsValue*);

bool
MmsValue_update(MmsValue* self, MmsValue* source);

void
MmsValue_setVisibleString(MmsValue* value, char* string);

MmsValue*
MmsValue_setUtcTime(MmsValue* value, uint32_t timeval);

//CHANGE _V_0_3_1_
MmsValue*
MmsValue_newDataAccessError(uint32_t code);

uint64_t
MmsValue_getUtcTimeInMs(MmsValue* value);

void
MmsValue_setBinaryTime(MmsValue* self, uint64_t timestamp);

uint64_t
MmsValue_getBinaryTimeAsUtcMs(MmsValue* value);

MmsType
MmsValue_getType(MmsValue* self);

MmsValue*
	MmsValue_newUtcTimeByTimestamp(uint32_t * timestamp);

char*
MmsValue_toString(MmsValue* value);

uint32_t
MmsValue_toUnixTimestamp(MmsValue* value);

MmsValue*
MmsValue_setUtcTimeMs(MmsValue* value, uint32_t * timestamp);

bool
MmsValue_isEqual(MmsValue* self, MmsValue* otherValue);

void
MmsValue_setUint8(MmsValue* value, uint8_t integer);

void
MmsValue_setUint16(MmsValue* value, uint16_t integer);

void
MmsValue_setInt32(MmsValue* value, int32_t integer);

void
MmsValue_setBoolean(MmsValue* value, bool boolValue);

bool
MmsValue_getBoolean(MmsValue* value);

void
MmsValue_setOctetString(MmsValue* self, uint8_t* buf, int size);

bool
MmsValue_isEqual(MmsValue* self, MmsValue* otherValue);

MmsValue*
MmsValue_newUtcTime(uint32_t timeval);

MmsValue*
MmsValue_newBitString(int bitSize);

uint32_t
MmsValue_toUint32(MmsValue* value);

void
MmsValue_setBitStringBit(MmsValue* self, int bitPos, bool value);

bool
MmsValue_getBitStringBit(MmsValue* self, int bitPos);

void
MmsValue_setAllBitStringBits(MmsValue* self);

void
MmsValue_deleteAllBitStringBits(MmsValue* self);

MmsValue*
MmsValue_newUnsignedFromUint32(uint32_t integer);

MmsValue*
MmsValue_newOctetString(int size, int maxSize);

MmsValue*
MmsValue_newBinaryTime(bool timeOfDay);

MmsValue*
MmsValue_newMmsString(char* string);

MmsValue*
MmsValue_newIntegerFromInt32(int32_t integer);


#endif /* __MMS_VALUE_H__ */
