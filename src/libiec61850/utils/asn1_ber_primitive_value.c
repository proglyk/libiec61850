#include <string.h>
#include "ied/utility/asn1_ber_primitive_value.h"
#include "stdlib.h"
#include "asn_integer.h"

static INTEGER_t mmsvalue;

Asn1PrimitiveValue*
Asn1PrimitiveValue_create(int size)
{
	Asn1PrimitiveValue* self = malloc(sizeof(Asn1PrimitiveValue));
	

	self->size = size;
	self->maxSize = size;
	self->octets = calloc(1, size);
	
//	self->octets[0] = 1;
	
	//mmsvalue.size = self->size;
	//mmsvalue.buf = self->octets;
//	asn_long2INTEGER(&mmsvalue, 0xffffffff);
	
	return self;
}

Asn1PrimitiveValue*
Asn1PrimitiveValue_clone(Asn1PrimitiveValue* self)
{
	Asn1PrimitiveValue* clone = malloc(sizeof(Asn1PrimitiveValue));

	clone->size = self->size;
	clone->maxSize = self->maxSize;

	clone->octets = malloc(self->maxSize);

	memcpy(clone->octets, self->octets, clone->maxSize);

	return clone;
}

int
Asn1PrimitiveValue_getSize(Asn1PrimitiveValue* self)
{
	return self->size;
}

int
Asn1PrimitiveValue_getMaxSize(Asn1PrimitiveValue* self)
{
	return self->maxSize;
}

void
Asn1PrimitiveValue_destroy(Asn1PrimitiveValue* self)
{
	free(self->octets);
	free(self);
}


bool
Asn1PrimitivaValue_compare(Asn1PrimitiveValue* self, Asn1PrimitiveValue* otherValue)
{
    if (self->size == otherValue->size) {
        if (memcmp(self->octets, otherValue->octets, self->size) == 0)
            return true;
        else
            return false;
    }
    else
        return false;
}
