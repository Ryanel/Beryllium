#include <log.h>

#ifdef X86
#include <drivers/low_textmode.h>
#include <drivers/bga.h>
#endif
#ifdef ARM

#endif

int video_device = 0; //Textmode x86

int video_graphics_init() //returns 0 if failed, 1 if sucessfull
{
	#ifdef X86
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
	#endif
	return 0;
}

int video_graphics_capable()
{
	return !bga_isavalable();
}

void video_printchar(int x,int y, unsigned char c)
{
	#ifdef X86
	textmode_write(x,y,c);
	#endif
	#ifdef ARM

	#endif
}

void video_printcoloredchar(int x,int y, unsigned char c, unsigned char attribute)
{
	#ifdef X86
	textmode_write_color(x,y,c, attribute);
	#endif
	#ifdef ARM

	#endif
}
void video_scroll(int from,int to)
{
	#ifdef X86
	textmode_scroll(from,to);
	#endif
}

void video_setcursor(int x,int y)
{
	#ifdef X86
	textmode_setcursor(x,y);
	#endif
}

void video_setattributetext(unsigned char back, unsigned char fore)
{
	#ifdef X86
	textmode_setcolor(back,fore);
	#endif
}
void video_resetattributetext()
{
	#ifdef X86
	textmode_resetcolor();
	#endif
}