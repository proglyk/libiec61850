#include "libiec61850/ied/map.h"
#include <stdint.h>
//#include "heap_user.h"


static int
comparePointerKeys(void* key1, void* key2);


Map
Map_create()
{
	Map map = calloc(1, sizeof(struct sMap));
	map->entries = LinkedList_create();
	map->compareKeys = comparePointerKeys;
	return map;
}


void*
Map_addEntry(Map map, void* key, void* value)
{
	MapEntry* entry = malloc(sizeof(MapEntry));
	entry->key = key;
	entry->value = value;
	LinkedList_add(map->entries, entry);

	return entry->key;
}


static int
comparePointerKeys(void* key1, void* key2)
{
	return (uint32_t)key2 - (uint32_t)key1;
}


void
Map_deleteDeep(Map map, bool deleteKey, void (*valueDeleteFunction) (void*))
{
	LinkedList element = map->entries;
	MapEntry* entry;

	while ((element = LinkedList_getNext(element)) != NULL) {
		entry = (MapEntry*) element->data;
		if (deleteKey == true)
			free(entry->key);
		valueDeleteFunction(entry->value);
	}

	LinkedList_destroy(map->entries);
	free(map);
}


void*
Map_getEntry(Map map, void* key)
{
	LinkedList element = map->entries;
	MapEntry* entry;

	while ((element = LinkedList_getNext(element)) != NULL) {
		entry = (MapEntry*) element->data;
		if (map->compareKeys(key, entry->key) == 0) {
			return entry->value;
		};
	}

	return NULL;
}


void*
Map_removeEntry(Map map, void* key, bool deleteKey)
{
	LinkedList element = map->entries;
	LinkedList lastElement = element;
	MapEntry* entry;
	void* value = NULL;

	while ((element = LinkedList_getNext(element)) != NULL) {
		entry = (MapEntry*) element->data;

		if (map->compareKeys(key, entry->key) == 0) {
			lastElement->next = element->next;
			value = entry->value;

			if (deleteKey == true)
				free(entry->key);
			free(entry);
			free(element);

			break;
		}

		lastElement = element;
	}

	return value;
}


