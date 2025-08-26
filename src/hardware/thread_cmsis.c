#include "server/thread.h"
#include "stdlib.h"
#include "temptypes.h"


static void threadRunner(void * argument);
static void threadRunnerAutoDest(void * argument);


struct sThread {
	ThreadExecutionFunction function;
	void* parameter;
	TaskHandle_t pthread;
	int state;
	bool autodestroy;
};

Thread
	Thread_create3(char * name, ThreadExecutionFunction func, void* arg, int ssize, osPriority
		sprio, bool autodestroy) {
	
	Thread thread = malloc(sizeof(struct sThread));
	TaskFunction_t task = NULL;
	
	thread->parameter = arg;
	thread->function = func;
	thread->state = 0;
	thread->autodestroy = autodestroy;
	
	if (autodestroy)
		task = &threadRunnerAutoDest;
	else
		task = &threadRunner;
	
	if (xTaskCreate(task, (const portCHAR *)name, ssize*configMINIMAL_STACK_SIZE, 
		(void *)thread, makeFreeRtosPriority(sprio),&(thread->pthread)) != pdPASS) {
			
		thread->pthread = (TaskHandle_t)NULL;
	}
	
	return thread;
}


Thread
	Thread_create2(ThreadExecutionFunction func, void* arg, int ssize, osPriority
		sprio) {
	
	Thread thread = malloc(sizeof(struct sThread));
	TaskFunction_t task = &threadRunner;;
	
	thread->parameter = arg;
	thread->function = func;
	thread->state = 0;
	thread->autodestroy = FALSE;
	
	if (xTaskCreate(task, (const portCHAR *)"pt0", ssize*configMINIMAL_STACK_SIZE, 
		(void *)thread, makeFreeRtosPriority(sprio),&(thread->pthread)) != pdPASS) {
			
		thread->pthread = (TaskHandle_t)NULL;
	}
	
	return thread;
}


Thread
Thread_create(ThreadExecutionFunction function, void* parameter, bool autodestroy)
{
	Thread thread = malloc(sizeof(struct sThread));
	TaskFunction_t task = NULL;
	
	thread->parameter = parameter;
	thread->function = function;
	thread->state = 0;
	thread->autodestroy = autodestroy;
	
	if (autodestroy)
		task = &threadRunnerAutoDest;
	else
		task = &threadRunner;
	
	if (xTaskCreate(task, (const portCHAR *)"pt0",configMINIMAL_STACK_SIZE, 
		(void *)thread, makeFreeRtosPriority(osPriorityNormal),
		&(thread->pthread)) != pdPASS) {
			
		thread->pthread = (TaskHandle_t)NULL;
	}
	
	return thread;
}


// в данной версии лишено смысла, т.к., создавая поток, xTaskCreate сразу его запускает.
void
Thread_start(Thread thread)
{
	thread->state = 1;
	//ResumeThread(thread->handle);
	osThreadResume(thread->pthread);
}


static void
threadRunner(void * argument){
	Thread thread = (Thread) argument;
	
	// osThreadSuspend Не работает почему-то. Сейчас она выключена дефайном INCLUDE_vTaskSuspend
//	osThreadSuspend(thread->pthread);
	thread->function(thread->parameter);
}


static void
threadRunnerAutoDest(void * argument){
	Thread thread = (Thread) argument;
	
// osThreadSuspend Не работает почему-то. Сейчас она выключена дефайном INCLUDE_vTaskSuspend
//	osThreadSuspend(thread->pthread);
	thread->function(thread->parameter);
	Thread_destroy(thread);
}


void
Thread_destroy(Thread thread){
	
	// osThreadSuspend Не работает почему-то. Сейчас она выключена дефайном INCLUDE_vTaskSuspend
//	osThreadSuspend(thread->pthread);
	osThreadTerminate(thread->pthread);
	free(thread);
}


void
Thread_sleep(int millies)
{
	vTaskDelay(millies);
}


Semaphore
Semaphore_create(int initialValue)
{
   /* Semaphore self = malloc(sizeof(sem_t));

    sem_init((sem_t*) self, 0, initialValue);

    return self;*/
}

/* Wait until semaphore value is greater than zero. Then decrease the semaphore value. */
void
Semaphore_wait(Semaphore self)
{
    //sem_wait((sem_t*) self);
}

void
Semaphore_post(Semaphore self)
{
    //sem_post((sem_t*) self);
}

void
Semaphore_destroy(Semaphore self)
{
    //sem_destroy((sem_t*) self);
    //free(self);
}
