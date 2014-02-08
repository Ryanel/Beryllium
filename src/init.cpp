#include <stdio.h>
#include <video.h>
#include <terminal.h>
#include <log.h>
#include <version.h>
#include <x86/x86.h>
#include <drivers/serial.h>
extern "C" void kernel_init()
{
	asm("cli");
	terminal_init();
	serial_init();
	init_x86();
	klog(LOG_INFO,"KERN","Finished initialising...\n");
	terminal_set_statusbar("Beryllium Unstable Isotope v. 0.0.0.1 (git)");
	video_init();
	asm("hlt");
}
