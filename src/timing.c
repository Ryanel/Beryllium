#include <types.h>
#include <log.h>
#include <beryllium/device.h>
#include <beryllium/watchdog.h>
#include <string.h>
#include <beryllium/timing.h>
#include <beryllium/timer.h>
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
    for(index = 0;index<0xFF;index++)
    {

        if(handlers[index].fire_tick == 0)
        {
            continue;
        }
        else
        {
            printf("[%02x]:%-10s calling at 0x%08X, repeating 0x%08X\n",index,handlers[index].name,handlers[index].fire_tick,handlers[index].repeat_rate);
        }
    }
}

int timing_init()
{
    memset(&handlers,0,sizeof(timer_t) * 0xFF);
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
        if(handlers[t_i].fire_tick < tick)
        {
            klog(LOG_DEBUG,"TMR","Removing old timer %d with tick 0x%X\n",t_i,handlers[t_i].fire_tick);
            handlers[t_i].fire_tick = 0;
            handlers[t_i].repeat_rate = 0;
            handlers[t_i].handler = 0;
        }
        else if(handlers[t_i].fire_tick == tick)
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
