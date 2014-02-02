///
/// Low Level x86 Functions -- Handles GDT, IDT, and stuff of that nature
///
#include <types.h>
#include <terminal.h>
#include <log.h>	
#include <low_cpu.h>
#include <x86.h>
#include <isr.h>
#include <low_paging.h>
void pit_install();
void init_x86()
{
	asm("cli");
	terminal_set_statusbar("x86: Initialising...");
	cpu_disableinterrupts();
	klog(LOG_DEBUG,"CPU","Interrupts disabled\n");
	terminal_set_statusbar("x86: Initialising GDT");
	gdt_setup();
	terminal_set_statusbar("x86: Initialising IDT");
	idt_setup();
	terminal_set_statusbar("x86: Initialising Interrupts");
	isrs_setup();
	irq_install();
	pit_install();
	paging_init();
	asm("sti");
}
