#include <log.h>
#include <mutex.h>
#ifdef X86
#include <x86/drivers/textmode.h>
#include <x86/drivers/bga.h>
#endif
#ifdef ARM

#endif

int video_device = 0; //Textmode x86

mutex_t *graphics_mutex;

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
	mutex_lock(graphics_mutex);
	textmode_write(x,y,c);
	mutex_unlock(graphics_mutex);
}

void video_printcoloredchar(int x,int y, unsigned char c, unsigned char attribute)
{
	mutex_lock(graphics_mutex);
	textmode_write_color(x,y,c, attribute);
	mutex_unlock(graphics_mutex);
}
void video_scroll(int from,int to)
{
	mutex_lock(graphics_mutex);
	textmode_scroll(from,to);
	mutex_unlock(graphics_mutex);
}

void video_setcursor(int x,int y)
{
	mutex_lock(graphics_mutex);
	textmode_setcursor(x,y);
	mutex_unlock(graphics_mutex);
}

void video_setattributetext(unsigned char back, unsigned char fore)
{
	mutex_lock(graphics_mutex);
	textmode_setcolor(back,fore);
	mutex_unlock(graphics_mutex);
	
}
void video_resetattributetext()
{
	mutex_lock(graphics_mutex);
	textmode_resetcolor();
	mutex_unlock(graphics_mutex);
	
}