///
/// Low Level x86 Functions -- Handles GDT, IDT, and stuff of that nature
///
#include <types.h>
#include <terminal.h>
#include <log.h>	
#include <x86/x86.h>
#include <x86/low/isr.h>
#include <x86/paging.h>
#include <beryllium/memory.h>
void pit_init();
void kb_init();
void init_x86()
{
	asm("cli");

	gdt_setup();

	idt_setup();

	isrs_setup();

	irq_install();

	pit_init();
	
	kb_init();

	paging_init();

	asm("sti");

	memory_init();
}
