#include <stdio.h>
#include <video.h>
#include <terminal.h>
#include <log.h>
#include <version.h>
#include <x86/x86.h>
#include <drivers/serial.h>
#include <boot/multiboot.h>

int memory_hi;
int memory_low;

extern void kernel_init(int magic,struct multiboot *mboot)
{
	#ifdef OPT_RELEASE
	klog_setmask(LOG_INFO);
	#endif
	asm("cli");

	terminal_init();
	#ifdef ENABLE_SERIAL
	serial_init();
	#endif
	if(magic!=0x2BADB002)
	{
		printf("Magic number : 0x%X! Attempting to boot anyway...\n",magic);
	}
	

	#ifdef DEBUG
	klog(LOG_WARN,"KERN","This kernel is a debug kernel! Some things might not work properly!\n");
	#endif

	memory_low = mboot->mem_lower;
	memory_hi = mboot->mem_upper;
	init_x86();

	klog(LOG_INFO,"KERN","Finished initialising...\n");
	terminal_set_statusbar("Beryllium Unstable Isotope v. 0.0.0.2 (git)");

	video_init();

	asm("hlt");
}
