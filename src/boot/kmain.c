#include <stdio.h>
#include <video.h>
#include <terminal.h>
#include <log.h>
#include <drivers/timer.h>
#include <version.h>
#include <stdlib.h>
#ifdef X86
void x86_switch_to_usermode();
#endif

void kmain()
{
	terminal_set_statusbar("Beryllium Unstable Isotope v. 0.0.0.2 (git)");
	klog(LOG_INFO,"KERN","CoreLibs initialising...\n");
	#ifdef DEBUG
	klog(LOG_WARN,"KERN","Running Debug Kernel! Some things might not work properly!\n");
	#endif
	

	klog(LOG_DEBUG,"IO","Verifiying timer / interrupts (waiting 10 ticks)\n");
	int timer_hi_orig = timer_getHi();
	while((timer_getHi() - timer_hi_orig) < 10)
	{
	}
	
	klog(LOG_INFO,"KERN","Finished initialising CoreLibs!\n");
	video_graphics_init();
	klog(LOG_INFO,"KERN","Reached end of kmain(); spinning...\n");
	while(1)
	{

	}
}
