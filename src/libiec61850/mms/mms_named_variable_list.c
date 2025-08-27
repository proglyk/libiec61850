#include "libiec61850/mms/mms_named_variable_list.h"

#include <stdlib.h>


static void
	deleteVariableListEntry(void* listEntry);


MmsNamedVariableList
MmsNamedVariableList_create(char* name, bool deletable)
{
	MmsNamedVariableList self = malloc(sizeof(struct sMmsNamedVariableList));

	self->deletable = deletable;
	self->name = copyString(name);
	self->listOfVariables = LinkedList_create();

	return self;
}

char*
MmsNamedVariableList_getName(MmsNamedVariableList self)
{
	return self->name;
}

bool
MmsNamedVariableList_isDeletable(MmsNamedVariableList self)
{
	return self->deletable;
}


void
MmsNamedVariableList_addVariable(MmsNamedVariableList self, MmsNamedVariableListEntry variable)
{
	LinkedList_add(self->listOfVariables, variable);
}

LinkedList
MmsNamedVariableList_getVariableList(MmsNamedVariableList self)
{
	return self->listOfVariables;
}

static void
deleteVariableListEntry(void* listEntry)
{
	MmsNamedVariableListEntry entry = (MmsNamedVariableListEntry) listEntry;
	MmsNamedVariableListEntry_destroy(entry);
}

void
MmsNamedVariableList_destroy(MmsNamedVariableList self)
{
	//CHANGE
	LinkedList_destroyDeep(self->listOfVariables, deleteVariableListEntry);
	free(self->name);
	free(self);
}


//CHANGE _V_0_3_0_
MmsNamedVariableListEntry
MmsNamedVariableListEntry_create(MmsAccessSpecifier accessSpecifier)
{
	MmsNamedVariableListEntry listEntry = malloc(sizeof(MmsAccessSpecifier));

	listEntry->domain = accessSpecifier.domain;
	listEntry->variableName = copyString(accessSpecifier.variableName);
	listEntry->arrayIndex = accessSpecifier.arrayIndex;

	if (accessSpecifier.componentName != NULL)
		listEntry->componentName = copyString(accessSpecifier.componentName);
	else
		listEntry->componentName = NULL;

	return listEntry;
}

//CHANGE
void
MmsNamedVariableListEntry_destroy(MmsNamedVariableListEntry self)
{
	free(self->variableName);
	free(self);
}

//CHANGE
MmsDomain*
MmsNamedVariableListEntry_getDomain(MmsNamedVariableListEntry self)
{
	return self->domain;
}

//CHANGE
char*
MmsNamedVariableListEntry_getVariableName(MmsNamedVariableListEntry self) {
	return self->variableName;
}


