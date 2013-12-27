#include <terminal.h>
#include <x86.h>
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
	print("Beryllium v. 0.0.0.1 (git) has booted\n");
	init_x86();
	asm("hlt");
}
