#include <stdio.h>
#include <beryllium/kmonitor.h>
#include <error.h>
#include <beryllium/timing.h>
void kmonitor_check(timer_t *value)
{
    //serial_printf("Called\n");
    return;
}

void kmonitor_init()
{
	timing_register_timer("kmonitor",10,kmonitor_check, 10);
}
