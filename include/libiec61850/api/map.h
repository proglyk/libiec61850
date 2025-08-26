/**
	******************************************************************************
  * @file    map.h
  * @author  Michael Zillgith
  * @brief   This file is part of libIEC61850.
  *****************************************************************************/

#ifndef __MAP_H__
#define __MAP_H__

#include "libiec61850/utils/linked_list.h"
#include <stdlib.h>
#include <stdbool.h>


typedef struct sMap* Map;
struct sMap {
	LinkedList entries;

	/* client provided function to compare two keys */
	int (*compareKeys)(void* key1, void* key2);
};


typedef struct sMapEntry {
	void* key;
	void* value;
} MapEntry;


Map
Map_create();

void*
Map_addEntry(Map map, void* key, void* value);

void
Map_deleteDeep(Map map, bool deleteKey, void (*valueDeleteFunction) (void*));

void*
Map_getEntry(Map map, void* key);

void*
Map_removeEntry(Map map, void* key, bool deleteKey);


#endif /* __MAP_H__ */
