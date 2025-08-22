#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "libiec61850/utils/linked_list.h"//#include "ied/utility/linked_list.h"

LinkedList
LinkedList_getLastElement(LinkedList list){
	while (list->next != NULL) {
		list = list->next;
	}
	return list;
}

LinkedList
LinkedList_create(){
	LinkedList newList;

	newList = malloc(sizeof(struct sLinkedList));
	newList->data = NULL;
	newList->next = NULL;

	return newList;
}

/**
 * Destroy list (free). Also frees element data with helper function.
 */

void
LinkedList_destroyDeep(LinkedList list, void (*valueDeleteFunction) (void*)){
	LinkedList nextElement = list;
	LinkedList currentElement;

	do {
		currentElement = nextElement;
		nextElement = currentElement->next;
		if (currentElement->data != NULL)
			valueDeleteFunction(currentElement->data);
		free(currentElement);
	} while (nextElement != NULL);
}

void
LinkedList_destroy(LinkedList list)
{
	LinkedList_destroyDeep(list, free);
}

/**
 * Destroy list (free) without freeing the element data
 */
void
LinkedList_destroyStatic(LinkedList list)
{
	LinkedList nextElement = list;
	LinkedList currentElement;

	do {
		currentElement = nextElement;
		nextElement = currentElement->next;
		free(currentElement);
	} while (nextElement != NULL);
}

int
LinkedList_size(LinkedList list)
{
	LinkedList nextElement = list;
	int size = 0;

	while (nextElement->next != NULL) {
		nextElement = nextElement->next;
		size++;
	}

	return size;
}

void
LinkedList_add(LinkedList list, void* data)
{
	LinkedList newElement = LinkedList_create();

	newElement->data = data;

	LinkedList listEnd = LinkedList_getLastElement(list);

	listEnd->next = newElement;
}

LinkedList inline
LinkedList_getNext(LinkedList list)
{
	return list->next;
}

void
LinkedList_printStringList(LinkedList list)
{
	LinkedList element = list;

	int elementCount = 0;
	char* str;

	while ((element = LinkedList_getNext(element)) != NULL) {
		str = (char*) (element->data);
		printf("%s\n", str);
		elementCount++;
	}
}

LinkedList
LinkedList_insertAfter(LinkedList list, void* data)
{
	LinkedList originalNextElement = LinkedList_getNext(list);

	LinkedList newElement = LinkedList_create();

	newElement->data = data;
	newElement->next = originalNextElement;

	list->next = newElement;

	return newElement;
}

