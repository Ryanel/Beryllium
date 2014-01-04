#include <terminal.h>
#include <x86.h>
#include <stdio.h>
#include <version.h>
void wait(int ii)
{
	int i=0;
	while(i!=ii)
	{
		i++;
	}
}
extern "C" void kernel_init()
{
	terminal_init();
	printf("Beryllium %s v. %s (%s) \n",BERYLLIUM_RELEASE,BERYLLIUM_VERSION,BERYLLIUM_SOURCE)
	init_x86();
	asm("hlt");
}
