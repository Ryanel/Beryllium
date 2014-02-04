#include <terminal.h>
#include <x86.h>
#include <log.h>
#include <version.h>
#include <drivers/serial.h>
#include <stdio.h>
#include <lib/liballoc.h>
extern "C" void kernel_init()
{
	asm("cli");
	terminal_init();
	serial_init();
	init_x86();
	klog(LOG_INFO,"KERN","Finished initialising...\n");
	terminal_set_statusbar("Beryllium Unstable Isotope v. 0.0.0.1 (git)");
	asm("hlt");
}
