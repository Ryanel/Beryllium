#include <thread.h>
#include <scheduler.h>
#include <stdlib.h>

thread_list_t *ready_queue = 0;
thread_list_t *current_thread = 0;

void scheduler_print_thread(thread_list_t *i)
{
	printf("[%d]\n",i->thread->id);
}

void scheduler_cycle()
{

	if(!ready_queue) return;
	thread_list_t *i = ready_queue;
	while(i->next)
	{
		i = i->next;
	}
	i->next = current_thread;
	current_thread = ready_queue;
	ready_queue = ready_queue->next;
	scheduler_print_thread(current_thread);
	thread_switch(current_thread);
}

void scheduler_init(thread_t *initial_thread)
{
	current_thread         = (thread_list_t*) malloc(sizeof (thread_list_t*));
	current_thread->thread = initial_thread;
	current_thread->next   = 0;
	ready_queue = 0;
}

void scheduler_print_threads()
{
	printf("Threads:\n");
	thread_list_t *i = ready_queue;
	while(i->next)
	{
		scheduler_print_thread(i);
		i = i->next;
	}
}

void scheduler_add(thread_t *thread)
{
	thread_list_t *item 	= (thread_list_t*) malloc(sizeof (thread_list_t*));
	item->thread 			= thread;
	item->next 				= 0;
	if(!ready_queue)
	{
		ready_queue = item;
	}
	else
	{
		thread_list_t *i = ready_queue;
		while(i->next)
		{
			i = i->next;
		}
		i->next = item;
	}
}
void scheduler_remove()
{

}

