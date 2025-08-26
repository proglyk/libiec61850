/**
	******************************************************************************
  * @file    mms_value_cache.c
  * @author  Michael Zillgith
  * @brief   This file is part of libIEC61850.
  *****************************************************************************/

#include "libiec61850/mms/mms_value_cache.h"
#include "libiec61850/utils/string_utilities.h"
#include <stdlib.h>
#include <string.h>
#include "libiec61850/utils/string_map.h"
#include "libiec61850/mms/mms_value.h"
#include "libiec61850/mms/mms_domain.h"
#include "libiec61850/mms/mms_type_spec.h"


/*******************************************************************************
	Locally viewed (static) routine prototypes
*******************************************************************************/


static char*
	getParentSubString(char* itemId);
	
static char*
	getChildSubString (char* itemId, char* parentId);
	
static void
	cacheEntryDelete(MmsValueCacheEntry* entry);
	
static MmsValue*
	searchCacheForValue(MmsValueCache self, char* itemId, char* parentId);


/*******************************************************************************
	Globally viewed (public) routine definitions
*******************************************************************************/


MmsValueCache
MmsValueCache_create(MmsDomain* domain)
{
	MmsValueCache self = calloc(1, sizeof(struct sMmsValueCache));

	self->domain = domain;

	self->map = StringMap_create();

	return self;
}

void
MmsValueCache_insertValue(MmsValueCache self, char* itemId, MmsValue* value)
{
	MmsTypeSpecification* typeSpec = MmsDomain_getNamedVariable(self->domain, 
		itemId);

	if (typeSpec != NULL) {
		MmsValueCacheEntry* cacheEntry = malloc(sizeof(MmsValueCacheEntry));

		cacheEntry->value = value;
		cacheEntry->typeSpec = typeSpec;

		Map_addEntry(self->map, copyString(itemId), cacheEntry);
	}
	//else
	//	if (DEBUG) printf("Cannot insert value into cache %s : no typeSpec found!\n", itemId);
}


void
MmsValueCache_destroy(MmsValueCache self)
{
	Map_deleteDeep(self->map, true, (void (*)(void *))cacheEntryDelete);
	free(self);
}


MmsValue*
MmsValueCache_lookupValue(MmsValueCache self, char* itemId)
{
	// get value for first matching key substring!
	// Then iterate the value for the exact value.

	MmsValue* value = NULL;

	MmsValueCacheEntry* cacheEntry;

	cacheEntry = (MmsValueCacheEntry*) Map_getEntry(self->map, itemId);

	if (cacheEntry == NULL) {
		char* itemIdCopy = copyString(itemId);
		char* parentItemId = getParentSubString(itemIdCopy);

		if (parentItemId != NULL) {
			value = searchCacheForValue(self, itemId, parentItemId);
		}

		free(itemIdCopy);
	}

	if (cacheEntry != NULL)
		return cacheEntry->value;
	else
		return value;
}


/*******************************************************************************
	Locally viewed (static) routines definition
*******************************************************************************/


static char*
getParentSubString(char* itemId)
{
	int len = strlen(itemId);

	char* strPos = itemId + len;

	while (--strPos > itemId) {
		if (*strPos == '$') {
			*strPos = 0;
			return itemId;
		}
	}

	return NULL;
}


static char*
getChildSubString (char* itemId, char* parentId)
{
	return itemId + strlen(parentId) + 1;
}


static MmsValue*
searchCacheForValue(MmsValueCache self, char* itemId, char* parentId)
{
	MmsValueCacheEntry* cacheEntry;
	MmsValue* value = NULL;

	cacheEntry = (MmsValueCacheEntry*) Map_getEntry(self->map, parentId);

	if (cacheEntry == NULL) {
		char* parentItemId = getParentSubString(parentId);

		if (parentItemId != NULL) {
			value = searchCacheForValue(self, itemId, parentItemId);
		}
	}
	else {
		char* childId = getChildSubString(itemId, parentId);
		MmsTypeSpecification* typeSpec = MmsDomain_getNamedVariable(self->domain, parentId);
		value = MmsTypeSpecification_getChildValue(typeSpec, cacheEntry->value, childId);
	}

	return value;
}



static void
cacheEntryDelete(MmsValueCacheEntry* entry)
{
	if (entry != NULL) {
		MmsValue_delete(entry->value);
		free(entry);
	}
}



