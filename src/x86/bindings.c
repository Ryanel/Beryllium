#include <boot/kmain.h>
#include <boot/multiboot.h>

#include <terminal.h>
#include <stdio.h>
#include <log.h>

#include <x86/x86.h>
#include <x86/drivers/serial.h>


/**
 Get the kernel up and running as fast as possible
**/
void kernel_x86_binding_init(volatile int magic,volatile struct multiboot *mboot)
{
	#ifdef IS_RELEASE
	klog_setmask(LOG_INFO);
	#endif

	terminal_init();

	#ifdef ENABLE_SERIAL
	serial_init();
	#endif
	klog(LOG_INFO,"x86","Loading x86 components...\n");
	if(magic!=0x2BADB002)
	{
		printf("Multiboot Magic number : 0x%X! It should equal 0x2BADB002. Halting...\n",magic);
		return;
	}
	init_x86();
	klog(LOG_INFO,"x86","Done starting hardware!\n");
	kmain();
}