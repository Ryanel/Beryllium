#include <types.h>
#include <low_paging.h>
#include <log.h>
#include <x86.h>
#include <isr.h>
#include <idt.h>
#include <irq.h>
#include <placement_malloc.h>
#include <string.h>

uint32_t *frames;
uint32_t nframes;

page_directory_t* kernel_directory;
page_directory_t* current_directory;
extern unsigned int* end;
extern unsigned int placement_address;
unsigned int page_aligned_end = (((unsigned int*)end) && 0xFFFFF000) + 0x1000;

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

// Static function to set a bit in the frames bitset
static void set_frame(uint32_t frame_addr)
{
	uint32_t frame = frame_addr/0x1000;
	uint32_t idx = INDEX_FROM_BIT(frame);
	uint32_t off = OFFSET_FROM_BIT(frame);
	frames[idx] |= (0x1 << off);
}

// Static function to clear a bit in the frames bitset
static void clear_frame(uint32_t frame_addr)
{
	uint32_t frame = frame_addr/0x1000;
	uint32_t idx = INDEX_FROM_BIT(frame);
	uint32_t off = OFFSET_FROM_BIT(frame);
	frames[idx] &= ~(0x1 << off);
}

// Static function to test if a bit is set.
static uint32_t test_frame(uint32_t frame_addr)
{
	uint32_t frame = frame_addr/0x1000;
	uint32_t idx = INDEX_FROM_BIT(frame);
	uint32_t off = OFFSET_FROM_BIT(frame);
	return (frames[idx] & (0x1 << off));
}

// Static function to find the first free frame.
static uint32_t first_frame()
{
	uint32_t i, j;
	for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
	{
		if (frames[i] != 0xFFFFFFFF) // nothing free, exit early.
		{
			// at least one bit is free here.
			for (j = 0; j < 32; j++)
			{
				uint32_t toTest = 0x1 << j;
				if ( !(frames[i]&toTest) )
				{
					return i*4*8+j;
				}
			}
		}
	}
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

	klog(LOG_PANIC,"PANIC","Page fault at 0x%X (present %d, rw %d, usermode %d, reserved %d)\n",faulting_address,present,rw,us,reserved);
	asm("cli");
	asm("hlt");
}

void paging_alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
	if(page->frame != 0)
	{
		return;
	}
	else
	{
		uint32_t idx = first_frame(); // idx is now the index of the first free frame.
		if (idx == (uint32_t)-1)
		{
			// PANIC is just a macro that prints a message to the screen then hits an infinite loop.
			klog(LOG_PANIC,"paging_alloc_frame","No free frames!\n");
		}
		set_frame(idx*0x1000); // this frame is now ours!
		page->present = 1; // Mark it as present.
		page->rw = (is_writeable)?1:0; // Should the page be writeable?
		page->user = (is_kernel)?0:1; // Should the page be user-mode?
		page->frame = idx;
	}
}

void paging_free_frame(page_t *page)
{
   uint32_t frame;
   if (!(frame=page->frame))
   {
       return; // The given page didn't actually have an allocated frame!
   }
   else
   {
       clear_frame(frame); // Frame is now free again.
       page->frame = 0x0; // Page now doesn't have a frame.
   }
}

page_t *get_page(uint32_t address, int make, page_directory_t *dir)
{
   // Turn the address into an index.
   address /= 0x1000;
   // Find the page table containing this address.
   uint32_t table_idx = address / 1024;
   if (dir->tables[table_idx]) // If this table is already assigned
   {
       return &dir->tables[table_idx]->pages[address%1024];
   }
   else if(make)
   {
       uint32_t tmp;
       dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), 1, &tmp);
       memset(dir->tables[table_idx], 0, 0x1000);
       dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
       return &dir->tables[table_idx]->pages[address%1024];
   }
   else
   {
       return 0;
   }
}

void paging_switch_directory(page_directory_t *dir)
{
	current_directory = dir;
	asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
	uint32_t cr0;
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000; // Enable paging!
	asm volatile("mov %0, %%cr0":: "r"(cr0));
}

void paging_init()
{
	//Clear page directory
	uint32_t mem_end_page = 0x1000000; //TODO: Make dynamic from data recieved from multiboot
	nframes = mem_end_page / 0x1000; //FIXME: Make sure mem_end_page is divisible by 0x1000
	frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(nframes));
	memset(frames, 0, INDEX_FROM_BIT(nframes));

	kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t),1);
	memset(kernel_directory, 0, sizeof(page_directory_t));
	current_directory = kernel_directory;

	int i = 0;
	while (i < placement_address)
	{
		// Kernel code is readable but not writeable from userspace.
		paging_alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
		i += 0x1000;
	}
	register_interrupt_handler(IRQ14, &paging_fault);
	paging_switch_directory(kernel_directory);
	klog(LOG_DEBUG,"CPU","Paging Setup (with %d frames)\n",nframes);
}
