#ifndef STRING_UTILITIES_H_
#define STRING_UTILITIES_H_

#include <stdint.h>

char* copyString(char* string);

/**
 * Concatenate strings. count indicates the number of strings
 * to concatenate.
 */
char* createString(int count, ...);

char* createStringFromBuffer(uint8_t* buf, int size);

#endif /* STRING_UTILITIES_H_ */
