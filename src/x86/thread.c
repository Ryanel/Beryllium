#include <thread.h>
#include <stdlib.h>
#include <stdio.h>
#include <scheduler.h>
#include <string.h>
uint32_t threading_nextid = 1;
thread_t *current_thread;

void thread_exit();

thread_t *threading_init()
{
	thread_t *thread = malloc(sizeof(thread_t));
	thread->id       = 0;
	//thread->name     = "kernel_main";
	current_thread   = thread;
	return thread;
}

thread_t *thread_create (int (*fn)(void*), void *arg, uint32_t *stack)
{
	thread_t *thread = malloc(sizeof(thread_t));
	memset(thread,0,sizeof(thread_t));
	thread->id       = threading_nextid++;			//Assign next id
	*--stack         = (uint32_t)arg;				//Argument
	*--stack         = (uint32_t)&thread_exit;		//Return address
	*--stack         = (uint32_t)fn;				//Function
	*--stack         = 0;
	thread->ebp      = 0;							//Assign Stack
	thread->esp      = (uint32_t)stack;				//^^^^^^^^^^^^
	thread->eflags   = 0x200;
	scheduler_add(thread);
	return thread;
}

thread_t *thread_create_named (int (*fn)(void*), void *arg, uint32_t *stack, char* name)
{
	thread_t *thread = malloc(sizeof(thread_t));
	memset(thread,0,sizeof(thread_t));
	thread->id       = threading_nextid++;			//Assign next id
	*--stack         = (uint32_t)arg;				//Argument
	*--stack         = (uint32_t)&thread_exit;		//Return address
	*--stack         = (uint32_t)fn;				//Function
	*--stack         = 0;
	thread->ebp      = 0;							//Assign Stack
	thread->esp      = (uint32_t)stack;				//^^^^^^^^^^^^
	thread->eflags   = 0x200;
	scheduler_add(thread);
	return thread;
}

/**
Performs Software Task Switching
**/
/*{
	asm volatile ("mov %%esp, %0" : "=r" (current_thread->esp));
	asm volatile ("mov %%ebp, %0" : "=r" (current_thread->ebp));
	asm volatile ("mov %%ebx, %0" : "=r" (current_thread->ebx));
	asm volatile ("mov %%esi, %0" : "=r" (current_thread->esi));
	asm volatile ("mov %%edi, %0" : "=r" (current_thread->edi));
	current_thread = to;
	asm volatile ("mov %0, %%esp" : :  "r" (to->esp));
	asm volatile ("mov %0, %%ebp" : :  "r" (to->ebp));
	asm volatile ("mov %0, %%ebx" : :  "r" (to->ebx));
	asm volatile ("mov %0, %%esi" : :  "r" (to->esi));
	asm volatile ("mov %0, %%edi" : :  "r" (to->edi));
}*/

void thread_exit()
{
	register uint32_t val asm("eax");
	printf("Thread exited with return value %d\n",val);
	for (;;) ;
}
