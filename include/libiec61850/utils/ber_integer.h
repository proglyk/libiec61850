#ifndef BER_INTEGER_H_
#define BER_INTEGER_H_

#include "libiec61850/utils/asn1_ber_primitive_value.h"

Asn1PrimitiveValue*
BerInteger_createFromBuffer(uint8_t* buf, int size);

Asn1PrimitiveValue*
BerInteger_createInt32();

int
BerInteger_setFromBerInteger(Asn1PrimitiveValue* self, Asn1PrimitiveValue* value);

int
BerInteger_setInt32(Asn1PrimitiveValue* self, int32_t value);

Asn1PrimitiveValue*
BerInteger_createFromInt32(int32_t value);

int
BerInteger_setUint16(Asn1PrimitiveValue* self, uint16_t value);

int
BerInteger_setUint32(Asn1PrimitiveValue* self, uint32_t value);

Asn1PrimitiveValue*
BerInteger_createFromUint32(uint32_t value);

Asn1PrimitiveValue*
BerInteger_createFromInt64(int64_t value);

Asn1PrimitiveValue*
BerInteger_createInt64();

int
BerInteger_setInt64(Asn1PrimitiveValue* self, int64_t value);

int /* 1 - if conversion is possible, 0 - out of range */
BerInteger_toInt32(Asn1PrimitiveValue* self, int32_t* nativeValue);

int /* 1 - if conversion is possible, 0 - out of range */
BerInteger_toUint32(Asn1PrimitiveValue* self, uint32_t* nativeValue);

int /* 1 - if conversion is possible, 0 - out of range */
BerInteger_toInt64(Asn1PrimitiveValue* self, int64_t* nativeValue);

int
BerInteger_setUint8(Asn1PrimitiveValue* self, uint8_t value);

#endif /* BER_INTEGER_H_ */
