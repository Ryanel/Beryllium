#include <terminal.h>

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
	print("Beryllium has booted\n");
	print("Starting components... (simulated)\n");
	//TODO: Start subsystems
	print("Initialising 1/7 subsystems...\n"); wait(90032000);
	print("Initialising 2/7 subsystems...\n"); wait(73435234);
	print("Initialising 3/7 subsystems...\n"); wait(85436563);
	print("Initialising 4/7 subsystems...\n"); wait(25254335);
	print("Initialising 5/7 subsystems...\n"); wait(62626262);
	print("Initialising 6/7 subsystems...\n"); wait(54354345);
	print("Initialising 7/7 subsystems...\n"); wait(63463463);
	print("Done!\n");
	wait(24454353);
	print("Launching init...\n"); wait(3032000);
	print("/bin/init: not found!\n"); wait(4032000);
	print("/boot/init: not found!\n"); wait(4532000);
	print("/sbin/init: not found!\n"); wait(4202000);
	print("/xbin/init: not found!\n"); wait(4332000);
	print("/bin/sh: not found!\n"); wait(4132000);
	print("/bin/bash: not found!\n"); wait(3832000);
	print("Couldn't find init, halting...\n");
	asm("hlt");
}
