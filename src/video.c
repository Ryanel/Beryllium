#include <log.h>

#ifdef X86
#include <x86/drivers/textmode.h>
#include <x86/drivers/bga.h>
#endif
#ifdef ARM

#endif

int video_device = 0; //Textmode x86

int video_graphics_init() //returns 0 if failed, 1 if sucessfull
{
	//bga_init();
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