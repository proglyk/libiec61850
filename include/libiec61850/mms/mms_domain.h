/**
	******************************************************************************
  * @file    mms_domain.h
  * @author  Michael Zillgith
  * @brief   This file is part of libIEC61850.
  *****************************************************************************/

#ifndef __MMS_DOMAIN_H__
#define __MMS_DOMAIN_H__

#include "libiec61850/mms/mms_types.h"
#include "libiec61850/utils/linked_list.h"
#include "libiec61850/mms/mms_named_variable_list.h"
#include "libiec61850/mms/mms_common.h"

struct sMmsDomain {
	char* domainName;
	int namedVariablesCount;
	MmsTypeSpecification** namedVariables;
	LinkedList namedVariableLists;
};

MmsDomain*
	MmsDomain_create(char* domainName);

void
	MmsDomain_destroy(MmsDomain* self);

MmsTypeSpecification*
	MmsDomain_getNamedVariable(MmsDomain* self, char* nameId);

MmsNamedVariableList
	MmsDomain_getNamedVariableList(MmsDomain* self, char* variableListName);

LinkedList
	MmsDomain_getNamedVariableLists(MmsDomain* self);
	
void
	MmsDomain_deleteNamedVariableList(MmsDomain* self, char* variableListName);
	
LinkedList
MmsDomain_getNamedVariableListValues(MmsDomain* self, char* variableListName);

LinkedList
MmsDomain_createNamedVariableListValues(MmsDomain* self, char* variableListName);

char*
MmsDomain_getName(MmsDomain* self);

bool
MmsDomain_addNamedVariableList(MmsDomain* self, MmsNamedVariableList variableList);

#endif /* __MMS_DOMAIN_H__ */
