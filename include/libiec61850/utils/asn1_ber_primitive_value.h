#ifndef ASN1_BER_PRIMITIVE_VALUE_H_
#define ASN1_BER_PRIMITIVE_VALUE_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	int size;
	int maxSize;
	uint8_t* octets;
} Asn1PrimitiveValue;

Asn1PrimitiveValue*
Asn1PrimitiveValue_create(int size);

int
Asn1PrimitiveValue_getSize(Asn1PrimitiveValue* self);

int
Asn1PrimitiveValue_getMaxSize(Asn1PrimitiveValue* self);

Asn1PrimitiveValue*
Asn1PrimitiveValue_clone(Asn1PrimitiveValue* self);

void
Asn1PrimitiveValue_destroy(Asn1PrimitiveValue* self);

bool
Asn1PrimitivaValue_compare(Asn1PrimitiveValue* self, Asn1PrimitiveValue* otherValue);

#endif /* ASN1_BER_PRIMITIVE_VALUE_H_ */
