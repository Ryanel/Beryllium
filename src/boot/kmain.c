#include <stdio.h>
#include <beryllium/video.h>
#include <terminal.h>
#include <log.h>
#include <beryllium/timer.h>
#include <system.h>
#include <stdlib.h>
#include <beryllium/vfs.h>
#include <beryllium/fs/device_man.h>
#include <beryllium/vterm.h>
#include <beryllium/thread.h>
#include <beryllium/driver.h>
#include <beryllium/watchdog.h>
#include <elf.h>
elf_t kernel_elf;
#ifdef X86
void x86_switch_to_usermode();
#endif
/**
Kernel main function
**/
void kmain()
{
	terminal_set_statusbar("Beryllium Unstable Isotope v. 0.0.0.4 (git)");
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
	
	klog(LOG_WARN,"KRN","Kernel init rescue shell launching -- no init found!\n");
	kshell_init();
	int i = 0;
	while(true)
	{
		kshell_parse_char(kb_read());
		wd_notify(WD_NOTIFY_KMAIN); //TODO: Make watchdog wrappers
	}

	panic("No init process to start (kernel init stub not compiled)!\n");
}
