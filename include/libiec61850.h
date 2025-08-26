#ifndef _LIBIEC61850_H_
#define _LIBIEC61850_H_

#define LIBIEC61850_VERSION	"0.3.3.0"	

// ISO_CONNECTION
// буфер приема
#define RECEIVE_BUF_SIZE 256
// буфер передачи. равен Размеру SRAM2
#define SEND_BUF_SIZE 0x4000

// макс. длина пути переменной в дереве. Это мой костыль, который стоит в 
// readAccessGooseControlBlock и представляет собой длину буфера variableId, 
// выделяемого стэком при вызове процедуры. Эта замена вынужденная, т.к. изна-
// чальный вариант функции, использующий malloc, терял память в размере 2 байта
// за вызов! (скорее всего длина строчки "GO", которая теряется, когда separator
// зануляется "*separator = 0")
#define MAX_VAR_PATH_LENGHT (long)64

#endif /*_LIBIEC61850_H_*/