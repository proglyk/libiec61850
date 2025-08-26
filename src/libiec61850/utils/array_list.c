#include "libiec61850/utils/array_list.h"
#include <stdlib.h>

int
ArrayList_listSize(void** list)
{
	int size = 0;

	while (list[size] != NULL)
		size++;
	return size;
}
