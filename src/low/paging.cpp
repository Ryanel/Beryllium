#include <types.h>
#include <low_paging.h>
#include <log.h>
#include <x86.h>
#include <isr.h>
#include <idt.h>
#include <irq.h>
#include <stdlib.h>
#include <string.h>
void paging_init()
{
	
}

void paging_fault(struct regs *regs)
{
	// A page fault has occurred.
	// The faulting address is stored in the CR2 register.
	uint32_t faulting_address;
	asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

	// The error code gives us details of what happened.
	int present   = !(regs->err_code & 0x1); // Page not present
	int rw = regs->err_code & 0x2;           // Write operation?
	int us = regs->err_code & 0x4;           // Processor was in user-mode?
	int reserved = regs->err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
	int id = regs->err_code & 0x10;          // Caused by an instruction fetch?

	klog(LOG_PANIC,"PANIC","Page fault at 0x%X (present %d, rw %d, usermode %d, reserved %d).\n Please send a bug report over on github if you recieve this message\n",faulting_address,present,rw,us,reserved);
	asm("cli");
	asm("hlt");
}
