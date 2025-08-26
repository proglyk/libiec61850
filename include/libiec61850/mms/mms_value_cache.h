/**
	******************************************************************************
  * @file    mms_value_cache.h
  * @author  Michael Zillgith
  * @brief   This file is part of libIEC61850.
  *****************************************************************************/

#ifndef MMS_VARIABLE_CACHE_H_
#define MMS_VARIABLE_CACHE_H_

#include "libiec61850/mms/mms_device.h"
#include "libiec61850/mms/mms_value_cache.h"
#include "libiec61850/ied/map.h"


/*******************************************************************************
	Types declarations
*******************************************************************************/


typedef struct sMmsValueCache {
	MmsDomain* domain;
	Map map;
} * MmsValueCache;

typedef struct sMmsValueCacheEntry {
	MmsValue* value;
	MmsTypeSpecification* typeSpec;
} MmsValueCacheEntry;


/*******************************************************************************
	Globally viewed routines prototypes
*******************************************************************************/


MmsValueCache
	MmsValueCache_create(MmsDomain* domain);

void
	MmsValueCache_insertValue(MmsValueCache self, char* itemId, MmsValue* value);

MmsValue*
	MmsValueCache_lookupValue(MmsValueCache self, char* itemId);

void
	MmsValueCache_destroy(MmsValueCache self);

#endif /* MMS_VARIABLE_CACHE_H_ */
