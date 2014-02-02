#include <types.h>
#include <low_paging.h>
#include <log.h>
#include <x86.h>
#include <isr.h>
#include <idt.h>
#include <irq.h>
#include <stdlib.h>
#include <string.h>
extern unsigned int* end;
unsigned int page_aligned_end = (((unsigned int*)end) && 0xFFFFF000) + 0x1000;
unsigned int *page_directory = (unsigned int*)page_aligned_end;

unsigned int *first_page_table = page_directory + 1024;

void paging_init()
{
	//Clear page directory
	int i = 0;
	int address=0;
	for(i = 0; i < 1024; i++)
	{
		//attribute: supervisor level, read/write, not present.
		page_directory[i] = 0 | 2; 
	}
	for(i = 0; i < 1024; i++)
	{
	    first_page_table[i] = address | 3; // attributes: supervisor level, read/write, present.
	    address = address + 4096; //advance the address to the next page boundary
	}
	page_directory[0] = *first_page_table;
	page_directory[0] |= 3;// attributes: supervisor level, read/write, present

	asm volatile("mov %0, %%cr3":: "b"(page_directory));
	unsigned int cr0;
	asm volatile("mov %%cr0, %0": "=b"(cr0));
	cr0 |= 0x80000000;
	asm volatile("mov %0, %%cr0":: "b"(cr0));

	register_interrupt_handler(IRQ14,&paging_fault);
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
