#include <stdio.h>
#include <video.h>
#include <terminal.h>
#include <log.h>
#include <version.h>
void kmain()
{
	klog(LOG_INFO,"KERN","Beryllium initialising...\n");
#ifdef DEBUG
	klog(LOG_WARN,"KERN","This kernel is a debug kernel! Some things might not work properly!\n");
#endif
    klog(LOG_INFO,"KERN","Finished initialising...\n");
	terminal_set_statusbar("Beryllium Unstable Isotope v. 0.0.0.2 (git)");
    
	video_graphics_init();
    
    //halt();
}
