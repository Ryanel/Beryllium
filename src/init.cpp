#include <terminal.h>
#include <x86.h>
#include <stdio.h>
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
	printf("Beryllium v. %s (%s) has booted\n","0.0.0.1","git");
	init_x86();
	asm("hlt");
}
