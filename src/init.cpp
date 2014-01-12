#include <terminal.h>
#include <x86.h>
#include <log.h>
#include <version.h>
#include <drivers/serial.h>
extern "C" void kernel_init()
{
	asm("cli");
	terminal_init();
	serial_init();
	serial_print("All messages will be relayed from the terminal to serial output\n");
	klog(LOG_INFO,"KERN","Beryllium %s v. %s (%s) \n",BERYLLIUM_RELEASE,BERYLLIUM_VERSION,BERYLLIUM_SOURCE);
	init_x86();
	klog(LOG_INFO,"KERN","Finished initialising...\n");
	klog(LOG_INFO,"KERN","Nothing to do: halting\n");
	asm("hlt");
}