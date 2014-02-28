#include <stdio.h>
#include <video.h>
#include <terminal.h>
#include <log.h>
#include <drivers/timer.h>
#include <version.h>

#ifdef X86
void x86_switch_to_usermode();
#endif

void enter_user_mode()
{
	#ifdef X86
	x86_switch_to_usermode();
	#endif
}

void kmain()
{
	terminal_set_statusbar("Beryllium Unstable Isotope v. 0.0.0.2 (git)");
	klog(LOG_INFO,"KERN","Beryllium initialising...\n");
	#ifdef DEBUG
	klog(LOG_WARN,"KERN","This kernel is a debug kernel! Some things might not work properly!\n");
	#endif
	

	klog(LOG_DEBUG,"IO","Verifiying timer / interrupts (waiting 2000 ticks)\n");
	int timer_hi_orig = timer_getHi();
	while((timer_getHi() - timer_hi_orig) < 2000)
	{
	}
	
	klog(LOG_INFO,"KERN","Finished basic initialisation\n");
	video_graphics_init();

	/* DISABLED USER MODE.
	klog(LOG_DEBUG,"KERN","Entering user mode...\n");

	enter_user_mode();
	*/
	while(1)
	{

	}
	//halt();
}
