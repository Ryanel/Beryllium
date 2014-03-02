#include <types.h>
#include <x86/low_paging.h>
#include <log.h>
#include <x86/page_allocator.h>
#include <x86/placement_malloc.h>
#include <string.h>
extern uint32_t end;

uint32_t eok_aligned; //End of kernel page aligned.

uint32_t *page_directory;
void paging_init()
{
	eok_aligned = (((uint32_t)&end) & 0xFFFFF000 ) + 0x1000;
	page_directory = (uint32_t*)eok_aligned;

	klog(LOG_DEBUG,"Paging","Page directory at:0x%X\n",eok_aligned);

	init_page_allocator();	
}