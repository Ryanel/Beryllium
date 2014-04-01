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
#include <scheduler.h>
#include <thread.h>
#include <driver.h>
#ifdef X86
void x86_switch_to_usermode();
#endif
/**
Kernel main function
**/

int fn()
{

}

void kmain()
{
	terminal_set_statusbar("Beryllium Unstable Isotope v. 0.0.0.3 (git)");
	klog(LOG_INFO,"KRN","CoreLibs initialising...\n");
	#ifdef DEBUG
	klog(LOG_WARN,"KRN","Running Debug Kernel! Some things might not work properly!\n");
	#endif
	
	device_manager_start();

	klog(LOG_DEBUG,"I/O","Verifiying timer / interrupts (waiting 10 ticks)\n");
	int timer_hi_orig = timer_getHi();
	while((timer_getHi() - timer_hi_orig) < 10)
	{
	}
	
	klog(LOG_INFO,"KRN","Finished initialising CoreLibs!\n");
	video_graphics_init();

	vfs_init();
	init_vfs_devices();
	
	for(;;); //Halt!
}