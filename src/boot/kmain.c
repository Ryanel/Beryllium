#include <stdio.h>
#include <video.h>
#include <terminal.h>
#include <log.h>
#include <timer.h>
#include <system.h>
#include <stdlib.h>
#include <vfs.h>
#include <fs/device_man.h>
#include <vterm.h>
#ifdef X86
void x86_switch_to_usermode();
#endif
/**
Kernel main function
**/
void kmain()
{
	terminal_set_statusbar("Beryllium Unstable Isotope v. 0.0.0.3 (git)");
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

	klog(LOG_DEBUG,"VFS","Starting VFS...\n");
	vfs_init();
	init_devices();

	klog(LOG_INFO,"KERN","Spinning...\n");

	term_t* term = term_create(80, 24, 0);
	term_writes(term,0,0,"Hello Virtual Terminal World!");
	while(1)
	{

	}
}
