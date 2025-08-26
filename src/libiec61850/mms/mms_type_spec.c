#include <stdlib.h>
#include <string.h>

#include "libiec61850/mms/mms_type_spec.h"

void
MmsTypeSpecification_delete(MmsTypeSpecification* typeSpec)
{
	if (typeSpec->name != NULL)
		free(typeSpec->name);

	if (typeSpec->type == MMS_STRUCTURE) {
		int elementCount = typeSpec->typeSpec.structure.elementCount;
		int i;
		for (i = 0; i < elementCount; i++) {
			MmsTypeSpecification_delete(typeSpec->typeSpec.structure.elements[i]);
		}

		free(typeSpec->typeSpec.structure.elements);
	}
	else if (typeSpec->type == MMS_ARRAY) {
		MmsTypeSpecification_delete(typeSpec->typeSpec.array.elementTypeSpec);
	}

	free(typeSpec);
}

static int
directChildStrLen(char* childId)
{
	int i = 0;
	int childIdLen = strlen(childId);
	while (i < childIdLen) {
		if (*(childId + i) == '$')
			break;
		i++;
	}

	return i;
}

//CHANGE _VER_0_3_0_
MmsValue*
MmsTypeSpecification_getChildValue(MmsTypeSpecification* typeSpec, MmsValue* value, char* childId)
{
	if (typeSpec->type == MMS_STRUCTURE) {
		int childLen = directChildStrLen(childId);
		int i;
		for (i = 0; i < typeSpec->typeSpec.structure.elementCount; i++) {

			if (strlen(typeSpec->typeSpec.structure.elements[i]->name) == childLen) {

				if (strncmp(typeSpec->typeSpec.structure.elements[i]->name, childId, childLen) == 0) {
					if (childLen == strlen(childId)) {
						return value->value.structure.components[i];
					}
					else {
						return MmsTypeSpecification_getChildValue(typeSpec->typeSpec.structure.elements[i],
								value->value.structure.components[i], childId + childLen + 1);
					}
				}
			}
		}
		return NULL;
	}
	else
		return NULL;
}
