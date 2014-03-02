///
/// Low Level x86 Functions -- Handles GDT, IDT, and stuff of that nature
///
#include <types.h>
#include <terminal.h>
#include <log.h>	
#include <x86/low_cpu.h>
#include <x86/x86.h>
#include <x86/isr.h>
#include <x86/paging.h>
void pit_install();

void init_x86()
{
	asm("cli");
	gdt_setup();
	idt_setup();
	isrs_setup();
	irq_install();
	pit_install();
	asm("sti");
	paging_init();
	
}
