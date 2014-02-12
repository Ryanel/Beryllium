#include <stdio.h>
#include <video.h>
#include <terminal.h>
#include <log.h>
#include <version.h>
#ifdef X86
#include <x86/x86.h>
#include <drivers/serial.h>
#include <boot/multiboot.h>
#endif
int memory_hi;
int memory_low;
#ifdef X86
void kernel_init_x86(int magic,struct multiboot *mboot)
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

	video_graphics_init();

	asm("hlt");
}
#endif
#ifdef ARM
void kernel_init_arm(int r0,int r1, int r2)
{

}
#endif
#ifdef X86
extern void kernel_init(int magic,struct multiboot *mboot)
#endif
#ifdef ARM
extern void kernel_init(int r0,int r1, int r2)
#endif
{
	#ifdef X86
	kernel_init_x86(magic, mboot);
	#endif
	#ifdef ARM
	kernel_init_arm(r0,r1,r2);
	#endif
}
