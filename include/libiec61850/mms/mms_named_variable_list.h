/**
	******************************************************************************
  * @file    mms_named_variable_list.h
  * @author  Michael Zillgith
  * @brief   This file is part of libIEC61850.
  *****************************************************************************/

#ifndef MMS_NAMED_VARIABLE_LIST_H_
#define MMS_NAMED_VARIABLE_LIST_H_


#include <stdbool.h>
#include "libiec61850/utils/linked_list.h"
#include "libiec61850/utils/string_utilities.h"
#include "libiec61850/mms/mms_common.h"

/*typedef struct sMmsNamedVariableList {
	bool deletable;
	char* name;
	LinkedList listOfVariables;
} * MmsNamedVariableList;

struct sMmsNamedVariableListEntry {
	MmsDomain* domain;
	char* variableName;
};*/

struct sMmsNamedVariableList {
	bool deletable;
	char* name;
	LinkedList listOfVariables;
};

char*
	MmsNamedVariableList_getName(MmsNamedVariableList self);

bool
	MmsNamedVariableList_isDeletable(MmsNamedVariableList self);

LinkedList
	MmsNamedVariableList_getVariableList(MmsNamedVariableList self);

MmsNamedVariableList
	MmsNamedVariableList_create(char* name, bool deletable);

void
	MmsNamedVariableList_addVariable(MmsNamedVariableList self, 
		MmsNamedVariableListEntry variable);

void
	MmsNamedVariableList_destroy(MmsNamedVariableList self);

//CHANGE _V_0_3_0_
MmsNamedVariableListEntry
MmsNamedVariableListEntry_create(MmsAccessSpecifier accessSpecifier);

//CHANG
void
	MmsNamedVariableListEntry_destroy(MmsNamedVariableListEntry self);

//CHANG
MmsDomain*
	MmsNamedVariableListEntry_getDomain(MmsNamedVariableListEntry self);

//CHANG
char*
	MmsNamedVariableListEntry_getVariableName(MmsNamedVariableListEntry self);


#endif /* MMS_NAMED_VARIABLE_LIST_H_ */
