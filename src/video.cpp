#include <drivers/bga.h>

#include <log.h>

int video_init() //returns 0 if failed, 1 if sucessfull
{
	if(!bga_isavalable())
	{
		klog(LOG_WARN,"VIDEO","BGA unsupported, setting terminal as output\n");
		return 0;
	}
	else
	{
		klog(LOG_INFO,"VIDEO","BGA avalable, setting terminal as output\n");
		return 0;
	}
	return 1;
}