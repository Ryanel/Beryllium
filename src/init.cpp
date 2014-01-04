#include <terminal.h>
#include <x86.h>
#include <stdio.h>
#include <version.h>
extern "C" void kernel_init()
{
	terminal_init();
	printf("Beryllium %s v. %s (%s) \n",BERYLLIUM_RELEASE,BERYLLIUM_VERSION,BERYLLIUM_SOURCE);
	init_x86();
	asm("hlt");
}
