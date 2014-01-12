#include <types.h>
#include <x86.h>
#include <ports.h>
#include <isr.h>
#include <error.h>
#include <idt.h> 
#include <log.h>
extern "C" void irq0();
extern "C" void irq1();
extern "C" void irq2();
extern "C" void irq3();
extern "C" void irq4();
extern "C" void irq5();
extern "C" void irq6();
extern "C" void irq7();
extern "C" void irq8();
extern "C" void irq9();
extern "C" void irq10();
extern "C" void irq11();
extern "C" void irq12();
extern "C" void irq13();
extern "C" void irq14();
extern "C" void irq15();

/* This array is actually an array of function pointers. We use
*  this to handle custom IRQ handlers for a given IRQ */
bool irq_routines[16] =
{
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

/* This installs a custom IRQ handler for the given IRQ */
void irq_install_handler(int irq)
{
	irq_routines[irq] = true;
}

/* This clears the handler for a given IRQ */
void irq_uninstall_handler(int irq)
{
	irq_routines[irq] = false;
}

void irq_remap(void)
{
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);
}

void irq_install()
{
	irq_remap();

	idt_set_gate(32, (unsigned)irq0, 0x08, 0x8E);
	idt_set_gate(33, (unsigned)irq1, 0x08, 0x8E);
	idt_set_gate(34, (unsigned)irq2, 0x08, 0x8E);
	idt_set_gate(35, (unsigned)irq3, 0x08, 0x8E);
	idt_set_gate(36, (unsigned)irq4, 0x08, 0x8E);
	idt_set_gate(37, (unsigned)irq5, 0x08, 0x8E);
	idt_set_gate(38, (unsigned)irq6, 0x08, 0x8E);
	idt_set_gate(39, (unsigned)irq7, 0x08, 0x8E);
	idt_set_gate(40, (unsigned)irq8, 0x08, 0x8E);
	idt_set_gate(41, (unsigned)irq9, 0x08, 0x8E);
	idt_set_gate(42, (unsigned)irq10, 0x08, 0x8E);
	idt_set_gate(43, (unsigned)irq11, 0x08, 0x8E);
	idt_set_gate(44, (unsigned)irq12, 0x08, 0x8E);
	idt_set_gate(45, (unsigned)irq13, 0x08, 0x8E);
	idt_set_gate(46, (unsigned)irq14, 0x08, 0x8E);
	idt_set_gate(47, (unsigned)irq15, 0x08, 0x8E);
	return;
}
void timer_handler(struct regs *r);
extern "C" void irq_handler(struct regs *r)
{
	/* This is a blank function pointer */
	bool handler;

	/* Find out if we have a custom handler to run for this
	*  IRQ, and then finally, run it */
	handler = irq_routines[r->int_no - 32];
	if (handler)
	{
		switch((r->int_no - 32))
		{
			case 0x0:
				timer_handler(r);
				break;
			default:
				klog(LOG_DEBUG,"IRQ","No handler installed for int 0x%X!\n",r->int_no - 32);
				//panic("Unhandled IRQ!\n");
				break;
		}
	}

	/* If the IDT entry that was invoked was greater than 40
	*  (meaning IRQ8 - 15), then we need to send an EOI to
	*  the slave controller */
	if (r->int_no >= 40)
	{
		outb(0xA0, 0x20);
	}

	/* In either case, we need to send an EOI to the master
	*  interrupt controller too */
	outb(0x20, 0x20);
}