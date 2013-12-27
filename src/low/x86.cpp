///
/// Low Level x86 Functions -- Handles GDT, IDT, and stuff of that nature
///
#include <types.h>
#include <terminal.h>
#include <low_cpu.h>
#include <x86.h>
void init_x86()
{
	terminal_set_statusbar("x86: Initialising... (0/3)");
	cpu_disableinterrupts();
	print("CPU: Interrupts disabled\n");
	terminal_set_statusbar("x86: Initialising... (1/3)");
	gdt_setup();
	terminal_set_statusbar("x86: Initialising... (2/3)");
	idt_setup();
	terminal_set_statusbar("x86: Initialising... (3/3)");
}
