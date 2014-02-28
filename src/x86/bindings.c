/**
 Get the kernel up and running as fast as possible
**/
#include <stdio.h>
#include <terminal.h>
#include <x86/x86.h>
#include <kmain.h>
#include <drivers/serial.h>
#include <boot/multiboot.h>
#include <log.h>
void kernel_x86_binding_init(int magic,struct multiboot *mboot)
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
	klog(LOG_INFO,"x86","Done\n");
	kmain();
}