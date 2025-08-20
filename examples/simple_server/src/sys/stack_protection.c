/*******************
 *
 * Copyright 2017 IAR Systems AB.
 *
 * This is a template implementation of support needed for
 * stack smash protection.
 *
 * Stack smash protection is implemented by placing a canary variable
 * between the return address and the rest of the functions stack frame.
 *
 ********************/

#include <stdlib.h>      /* For void abort(void) */
#include <stdint.h>      /* For uint32_t */
#include <stdio.h>
#include "stm32f4xx.h"
 

#pragma language=extended


void __init_stack_chk_guard(void);


/*
 * This variable holds the value that the canary will be initialized with
 * and also compared with before returning from the function.
 */
#pragma required=__init_stack_chk_guard
__no_init uint32_t __stack_chk_guard;

/*
 * The __stack_chk_fail(void) function is called when a modified canary is detected.
 */
#pragma no_stack_protect
__attribute__((noreturn)) void __stack_chk_fail(void)
{

	//printf("__stack_chk_fail: Fail\r\n");
	
  /*
  LogStackSmashed();
  */
  abort();
}

/*
 * This function gets called at startup as part of construction of static C++ objects
 * and is a way to programatically initialize the __stack_chk_guard when the system starts.
 */
#pragma early_dynamic_initialization
#pragma no_stack_protect
__attribute__((constructor)) void __init_stack_chk_guard(void)
{
  __stack_chk_guard = 4711;
}
