#ifndef THREAD_H_
#define THREAD_H_

#include <stdbool.h>
//#include "cmsis_os.h"

typedef struct sThread* Thread;
typedef void (*ThreadExecutionFunction) (void*);

typedef void* Semaphore;

Thread Thread_create(ThreadExecutionFunction, void*, bool);
Thread Thread_create2(ThreadExecutionFunction, void*, int, int);
Thread Thread_create3(char *, ThreadExecutionFunction, void*, int, int, bool);
		
void Thread_start(Thread thread);
void Thread_destroy(Thread thread);
void Thread_sleep(int millies);

Semaphore	Semaphore_create(int initialValue);

/* Wait until semaphore value is greater than zero. Then decrease the semaphore value. */
void Semaphore_wait(Semaphore self);
void Semaphore_post(Semaphore self);
void Semaphore_destroy(Semaphore self);

#endif /* THREAD_H_ */
