#include <types.h>
#include <beryllium/device.h>
#include <beryllium/watchdog.h>
int cyclic_tasks(int tick)
{
	if(tick % 10 == 0)
	{
		wd_evaluate();
	}
	return 0;
}