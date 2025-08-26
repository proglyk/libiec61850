/**
	******************************************************************************
  * @file    string_map.c
  * @author  Michael Zillgith
  * @brief   This file is part of libIEC61850.
  *****************************************************************************/

#include "libiec61850/utils/string_map.h"
#include <string.h>

Map
StringMap_create() {
	Map map = Map_create();
	map->compareKeys = (int(*)(void*, void*))strcmp;
	return map;
}
