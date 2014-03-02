#include <stdio.h>
#include <video.h>
#include <terminal.h>
#include <log.h>
#include <drivers/timer.h>
#include <version.h>
#ifdef X86
void x86_switch_to_usermode();
#endif

void kmain()
{
	terminal_set_statusbar("Beryllium Unstable Isotope v. 0.0.0.2 (git)");
	klog(LOG_INFO,"KERN","Beryllium initialising...\n");
	#ifdef DEBUG
	klog(LOG_WARN,"KERN","This kernel is a debug kernel! Some things might not work properly!\n");
	#endif
	

	klog(LOG_DEBUG,"IO","Verifiying timer / interrupts (waiting 10 ticks)\n");
	int timer_hi_orig = timer_getHi();
	while((timer_getHi() - timer_hi_orig) < 10)
	{
	}
	
	klog(LOG_INFO,"KERN","Finished basic initialisation\n");
	video_graphics_init();
	memory_init();

	while(1)
	{

	}
}
