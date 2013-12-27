#include <string.h>
#include <types.h>
#include <idt.h>
#include <terminal.h>
#include <stdio.h>
#include <x86.h>
struct idt_entry idt[256];
struct idt_ptr idtp;

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}


int idt_setup()
{
	idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
	idtp.base = (unsigned int)&idt;

	memset(&idt, 0, sizeof(struct idt_entry) * 256);

	isrs_setup();

	idt_load();
	printf("CPU: Interrupts are Setup\n");
	asm("sti");
	printf("CPU: Interrupts enabled\n");
	print("CPU: IDT Setup\n");
	return 0;
}
