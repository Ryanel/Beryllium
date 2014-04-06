#include <types.h>
#include <log.h>
#include <beryllium/device.h>
#include <beryllium/watchdog.h>
#include <string.h>
#include <beryllium/timing.h>
#include <beryllium/timer.h>
#include <stdio.h>
timer_t handlers[0xFF];//We assume that we will never need more than 0xFF, but we can change that in the future to rely on malloc

int timing_register_timer(char *name,int tick,void (*handler)(timer_t *), int reschedule)
{
	klog(LOG_DEBUG,"TMR","Scheduling %s at tick 0x%X, and rescheduling every %d ticks\n",name,tick,reschedule);
	int index = 0;
	for(index = 0;index<0xFF;index++)
	{

		if(handlers[index].fire_tick == 0)
		{
			break;
		}
		else
		{
			continue;
		}
	}
	if(index == 0xFF)
	{
		klog(LOG_ERROR,"Failed to register timer %s!\n",name);
		return 0xFF;
	}
	timer_t builder;
	builder.fire_tick = tick + timer_getHi();
	builder.name = name;
	builder.repeat_rate = reschedule;
	builder.handler = handler;
	builder.no = index;
	handlers[index] = builder;
	return index;

}

void timing_deregister_timer(void (*handler)(timer_t *))
{
	for(int index = 0;index<0xFF;index++)
	{

		if(handlers[index].handler == 0)
		{
			continue;
		}
		else
		{
			if(handlers[index].handler == handler)
			{
				handlers[index].handler = 0;
				handlers[index].repeat_rate = 0;
				handlers[index].fire_tick = 0;
				break;
			}
			continue;
		}
	}
}

void list_timers()
{
	int index = 0;
	int current_time = timer_getHi();
	for(index = 0;index<0xFF;index++)
	{

		if(handlers[index].fire_tick == 0)
		{
			continue;
		}
		else
		{
			int rep_in = handlers[index].fire_tick - current_time;
			printf("[%02x]:%-15s calling at %-8d, repeating %-8d (in %d ticks)\n",index,handlers[index].name,handlers[index].fire_tick,handlers[index].repeat_rate,rep_in);
		}
	}
}

void timing_reaper()
{
	int t_i = 0;
	int tick = timer_getHi();
	for (t_i = 0; t_i < 0xFF; t_i++)
	{
		if(handlers[t_i].fire_tick == 0)
		{
			continue;
		}
		if(handlers[t_i].fire_tick < tick)
		{
			klog(LOG_DEBUG,"TRR","Removing old timer %d (%s) with tick 0x%X\n",t_i,handlers[t_i].name,handlers[t_i].fire_tick);
			handlers[t_i].fire_tick = 0;
			handlers[t_i].repeat_rate = 0;
			handlers[t_i].handler = 0;
		}
	}
}

int timing_init()
{
	memset(&handlers,0,sizeof(timer_t) * 0xFF);
	timing_register_timer("timer_reaper",0xF,timing_reaper, 1000);
	return 0;
}
int t_i = 0;
int cyclic_tasks(int tick)
{
	t_i = 0;
	for (t_i = 0; t_i < 0xFF; t_i++)
	{
		if(handlers[t_i].fire_tick == 0)
		{
			continue;
		}
		if(handlers[t_i].fire_tick == tick)
		{
			handlers[t_i].handler(&handlers[t_i]);
			if(handlers[t_i].repeat_rate != 0)
			{
				handlers[t_i].fire_tick = tick + handlers[t_i].repeat_rate;
			}
		}
	}
	return 0;
}
