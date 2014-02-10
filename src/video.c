#include <drivers/bga.h>

#include <log.h>

int video_init() //returns 0 if failed, 1 if sucessfull
{
	if(!bga_isavalable())
	{
		klog(LOG_WARN,"VIDEO","BGA unsupported, setting terminal as output\n");
		return 0;
	}
	#ifdef ENABLE_VIDEO
	else
	{
		klog(LOG_INFO,"VIDEO","BGA avalable, setting BGA framebuffer as output\n");
		bga_init();
		return 0;
	}
	#endif
	return 1;
}

