#include "libiec61850/utils/string_utilities.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

char* copyString(char* string) {
	int newStringLength = strlen(string) + 1;

	char* newString = malloc(newStringLength);

	memcpy(newString, string, newStringLength);

	return newString;
}

char* createString(int count, ...) {
	va_list ap;
	char* newStr;
	char* currentPos;
	int newStringLength = 0;
	int i;

	/* Calculate new string length */
	va_start(ap, count);
	for (i = 0; i < count; i++) {
		char* str = va_arg(ap, char*);

		newStringLength += strlen(str);
	}
	va_end(ap);

	newStr = malloc(newStringLength + 1);
	currentPos = newStr;


	va_start(ap, count);
	for (i = 0; i < count; i++) {
		char* str = va_arg(ap, char*);
		strcpy(currentPos, str);
		currentPos += strlen(str);
	}
	va_end(ap);

	*currentPos = 0;

	return newStr;
}

char* createStringFromBuffer(uint8_t* buf, int size)
{
	char* newStr = malloc(size + 1);

	memcpy(newStr, buf, size);
	newStr[size] = 0;

	return newStr;
}

