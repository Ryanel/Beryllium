#include <terminal.h>
#include <low_cpu.h>
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
	terminal_set_statusbar("Beryllium initialisation");
	print("Beryllium has booted\n");
	cpu_disableinterrupts();
	print("CPU: Interrupts disabled\n");
	//TODO: Start subsystems
	terminal_set_statusbar("Beryllium initialisation -- halted");
	asm("hlt");
}
