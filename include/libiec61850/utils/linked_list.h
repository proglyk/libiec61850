#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

struct sLinkedList {
	void* data;
	struct sLinkedList* next;
};

typedef struct sLinkedList* LinkedList;

LinkedList LinkedList_create();

void LinkedList_destroy(LinkedList list);

void LinkedList_add(LinkedList list, void* data);

void LinkedList_remove(void* data);

void LinkedList_removeAtIndex(int index);

LinkedList LinkedList_getNext(LinkedList list);

int LinkedList_size(LinkedList list);

void
LinkedList_destroyStatic(LinkedList list);

void
LinkedList_printStringList(LinkedList list);

void
LinkedList_destroyDeep(LinkedList list, void (*valueDeleteFunction) (void*));

LinkedList
LinkedList_getLastElement(LinkedList list);

LinkedList
LinkedList_insertAfter(LinkedList list, void* data);

#endif /* LINKED_LIST_H_ */
