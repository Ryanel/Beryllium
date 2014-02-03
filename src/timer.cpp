#include <log.h>
#include <types.h>
#include <drivers/timer.h>
#include <ktaskman.h>
/** High resolution timer ticks. Set every single clock tick **/
uint32_t timer_hi = 0;
/** Low resolution timer ticks/ Set every single overflow of timer_hi **/
uint32_t timer_low = 0;
uint32_t device = 0; //0 - pit, 1 - rtc. Used as the source. Will be replaced by a driver_t;
uint32_t device_resoulution = 1; //How many ticks make a millisecond

void switchDevice(uint32_t device_id)
{
	switch (device_id)
	{
		case 0x0:
			device = 0;
			device_resoulution = 1;
			break;
		default:
			switchDevice(0);
			break;
	}
}

void recieveTick(uint32_t device_id)
{
	if(device_id==device)
	{
		timer_hi++;
		if((timer_hi % (device_resoulution * 1000)) == 0)
		{
			klog(LOG_DEBUG,"CTimer","Device %d, resolution = %d (tick no %d)\n",device_id,device_resoulution,timer_hi);
			timer_low++;
		}
		cyclic_tasks(timer_hi);
	}
}

uint32_t getTimerHi()
{
	return timer_hi;
}

uint32_t getTimerLow()
{
	return timer_low;
}

int getUptime()
{
	return timer_hi/(device_resoulution*1000);
}