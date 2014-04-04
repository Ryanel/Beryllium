#include <types.h>
#include <beryllium/device.h>
unsigned int kernel_main_wd = 0;
unsigned int kernel_main_wd_old = 0;
unsigned int kernel_main_wd_transgressions = 0;
int cyclic_tasks(int tick)
{
	if(tick % 10 == 0)
	{
		if(kernel_main_wd == kernel_main_wd_old)
		{
			if(kernel_main_wd_transgressions == 100)
			{
				panic("Watchdog reported that the kernel main thread hung!\n");
			}
			kernel_main_wd_transgressions++;
		}
	}
	kernel_main_wd_old = kernel_main_wd;
	return 0;
}