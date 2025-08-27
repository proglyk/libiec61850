/**
	******************************************************************************
  * @file    mms_domain.ñ
  * @author  Michael Zillgith
  * @brief   This file is part of libIEC61850.
  *****************************************************************************/

#include "libiec61850/mms/mms_device.h"
#include "libiec61850/utils/string_utilities.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "libiec61850/mms/mms_named_variable_list.h"
#include <stdbool.h>
#include "libiec61850/mms/mms_server_common.h"
//#include "heap_user.h"


static void
freeNamedVariables(MmsTypeSpecification** variables, int variablesCount);


//CHANGE _V_0_3_2_
static MmsTypeSpecification*
getNamedVariableRecursive(MmsTypeSpecification* variable, char* nameId)
{
	char* separator = strchr(nameId, '$');

	int i;

	if (separator == NULL) {

		i = 0;

		if (variable->type == MMS_STRUCTURE) {
			for (i = 0; i < variable->typeSpec.structure.elementCount; i++) {
				if (strcmp(variable->typeSpec.structure.elements[i]->name, nameId) == 0) {
					return variable->typeSpec.structure.elements[i];
				}
			}
		}

		return NULL;
	}
	else {
		MmsTypeSpecification* namedVariable = NULL;
		i = 0;

		for (i = 0; i < variable->typeSpec.structure.elementCount; i++) {

			if (strlen(variable->typeSpec.structure.elements[i]->name) == (separator - nameId)) {

				if (strncmp(variable->typeSpec.structure.elements[i]->name, nameId, separator - nameId) == 0) {
					namedVariable = variable->typeSpec.structure.elements[i];
					break;
				}

			}
		}

		if (namedVariable != NULL) {
		    if (namedVariable->type == MMS_STRUCTURE) {
		        namedVariable = getNamedVariableRecursive(namedVariable, separator + 1);
		    }
		    else if (namedVariable->type == MMS_ARRAY) {
		        namedVariable = namedVariable->typeSpec.array.elementTypeSpec;

		        namedVariable = getNamedVariableRecursive(namedVariable, separator + 1);
		    }
		}

		return namedVariable;
	}
}

//CHANGE _V_0_3_0_
static void
freeNamedVariables(MmsTypeSpecification** variables, int variablesCount)
{
	int i;
	for (i = 0; i < variablesCount; i++) {
		if (variables[i]->name != NULL)
			free(variables[i]->name);

		if (variables[i]->type == MMS_STRUCTURE) {
			freeNamedVariables(variables[i]->typeSpec.structure.elements,
					variables[i]->typeSpec.structure.elementCount);
			free(variables[i]->typeSpec.structure.elements);
		}
		else if (variables[i]->type == MMS_ARRAY) {
			freeNamedVariables(&(variables[i]->typeSpec.array.elementTypeSpec), 1);
		}
		free(variables[i]);
	}
}

MmsDomain*
MmsDomain_create(char* domainName)
{
	MmsDomain* self = calloc(1, sizeof(MmsDomain));

	self->domainName = copyString(domainName);
	self->namedVariableLists = LinkedList_create();

	return self;
}

void
MmsDomain_destroy(MmsDomain* self)
{
	free(self->domainName);

	if (self->namedVariables != NULL) {
		freeNamedVariables(self->namedVariables,
				self->namedVariablesCount);

		free(self->namedVariables);
	}

	LinkedList_destroyDeep(self->namedVariableLists, (void (*)(void *))MmsNamedVariableList_destroy);

	free(self);
}

bool
MmsDomain_addNamedVariableList(MmsDomain* self, MmsNamedVariableList variableList)
{
	//TODO check if operation is allowed!

	LinkedList_add(self->namedVariableLists, variableList);

	return true;
}

MmsNamedVariableList
MmsDomain_getNamedVariableList(MmsDomain* self, char* variableListName)
{
	MmsNamedVariableList variableList = NULL;

	LinkedList element = LinkedList_getNext(self->namedVariableLists);

	while (element != NULL) {
		MmsNamedVariableList varList = (MmsNamedVariableList) element->data;

		if (strcmp(MmsNamedVariableList_getName(varList), variableListName) == 0) {
			variableList = varList;
			break;
		}

		element = LinkedList_getNext(element);
	}

	return variableList;
}

//CHANGE VER_0_2_0
void
MmsDomain_deleteNamedVariableList(MmsDomain* self, char* variableListName)
{
	mmsServer_deleteVariableList(self->namedVariableLists, variableListName);
}

//CHANGE VER_0_2_0
/*void
MmsDomain_deleteNamedVariableList(MmsDomain* self, char* variableListName)
{
	LinkedList previousElement = self->namedVariableLists;
	LinkedList element = LinkedList_getNext(self->namedVariableLists);

	while (element != NULL) {
		MmsNamedVariableList varList = (MmsNamedVariableList) element->data;

		if (strcmp(MmsNamedVariableList_getName(varList), variableListName) == 0) {
			previousElement->next = element->next;
			free(element);
			MmsNamedVariableList_destroy(varList);

			break;
		}

		previousElement = element;
		element = LinkedList_getNext(element);
	}
}*/

LinkedList
MmsDomain_getNamedVariableLists(MmsDomain* self)
{
	return self->namedVariableLists;
}


//CHANGE _V_0_3_0_
MmsTypeSpecification*
MmsDomain_getNamedVariable(MmsDomain* self, char* nameId)
{
	if (self->namedVariables != NULL) {

		char* separator = strchr(nameId, '$');

		int i;

		if (separator == NULL) {

			for (i = 0; i < self->namedVariablesCount; i++) {
				if (strcmp(self->namedVariables[i]->name, nameId) == 0) {
					return self->namedVariables[i];
				}
			}

			return NULL;
		}
		else {
			MmsTypeSpecification* namedVariable = NULL;

			for (i = 0; i < self->namedVariablesCount; i++) {

				if (strlen(self->namedVariables[i]->name) == (separator - nameId)) {

					if (strncmp(self->namedVariables[i]->name, nameId, separator - nameId) == 0) {
						namedVariable = self->namedVariables[i];
						break;
					}
				}
			}

			if (namedVariable != NULL) {
				namedVariable = getNamedVariableRecursive(namedVariable, separator + 1);
			}

			return namedVariable;
		}
	}
	return NULL;
}

// CHANGE VER_0_2_0
char*
MmsDomain_getName(MmsDomain* self)
{
	return self->domainName;
}

