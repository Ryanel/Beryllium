#include <log.h>
#include <mutex.h>
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
	textmode_write(x,y,c);
}

void video_printcoloredchar(int x,int y, unsigned char c, unsigned char attribute)
{
	textmode_write_color(x,y,c, attribute);
}
void video_scroll(int from,int to)
{
	textmode_scroll(from,to);
}

void video_setcursor(int x,int y)
{
	textmode_setcursor(x,y);
}

void video_setattributetext(unsigned char back, unsigned char fore)
{
	textmode_setcolor(back,fore);
}
void video_resetattributetext()
{
	textmode_resetcolor();
}