/**
 Get the kernel up and running as fast as possible
**/
#include <stdio.h>
#include <terminal.h>
#include <x86/x86.h>
#include <kmain.h>
void kernel_x86_binding_init(int magic,struct multiboot *mboot)
{
    klog_setmask(LOG_INFO);
    terminal_init();
#ifdef ENABLE_SERIAL
	serial_init();
#endif
    klog(LOG_INFO,"x86","Loading x86 components...\n");
    if(magic!=0x2BADB002)
	{
		printf("Magic number : 0x%X! Halting...\n",magic);
        return;
	}
    init_x86();
    klog(LOG_INFO,"x86","Done\n");
    kmain();
}