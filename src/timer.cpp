#include <log.h>
#include <types.h>
#include <ktaskman.h>
/** High resolution timer ticks. Set every single clock tick **/
uint32_t timer_hi = 0;
/** Low resolution timer ticks/ Set every single overflow of timer_hi **/
uint32_t timer_low = 0;
uint32_t device = 0; //0 - pit, 1 - rtc. Used as the source. Will be replaced by a driver_t;
uint32_t device_resoulution = 1; //How many ms is a tick

void recieveTick(uint32_t device_id)
{
	if(device_id==device)
	{
		timer_hi++;
		if((timer_hi % 1000) == 0)
		{
			klog(LOG_DEBUG,"CTimer","Device %d, resolution = %d (tick no %d)\n",device_id,device_resoulution,timer_hi);
		}
		if((timer_hi % 10) == 0)
		{
			cyclic_tasks(timer_hi);
		}
	}
}

int getUptime()
{
	return timer_hi/(device_resoulution*1000);
}