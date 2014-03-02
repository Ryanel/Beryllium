#include <x86/paging.h>
#include <log.h>
#include <x86/page_allocator.h>
#include <x86/placement_malloc.h>
#include <types.h>
#include <stdio.h>
#include <x86/memory.h>
#include <mutex.h>
//Allocates multiple pages
extern page_directory_t *kernel_directory;
int mem_pagefree = 0xA0000000; //Default value, dynamic get in future!
int mem_lastpage = 0;

mutex_t *mmac_lock;

void* memory_mult_alloc_pages(int pages)
{
	int i, j;
	int block = -1;
  	for(i = 0; i < pa_frame_amount(); i++)
  	{
  		int pos = (i + mem_lastpage) % pa_frame_amount(); // Create a position based off of i.
		if ( (pos + pages) > pa_frame_amount() )
		{
			i += pages - 2;
			continue;	
		}
		block = pos;
		
		for(j = 0;j < pages; j++)
		{
			if ( pa_test_frame(INDEX_FROM_BIT(pos + j)) ) 
			{
				block = -1;
				i = i + j;
				break;
			}
		}
		if ( block != -1 ) break;

  	}
  	if ( block == -1 ) return 0;
  	for ( i = 0; i < pages; i++ )
		//pa_set_frame(INDEX_FROM_BIT(block + i));
		pa_alloc_frame(paging_get_page(block + i,1, kernel_directory), 0, 0);

	mem_lastpage = block + pages;

	return (void*)((unsigned int)block);
}

void* memory_alloc_pages(int pages)
{
	void *ptr = NULL;
	if(pages <= 0)
	{
		return 0;
	}

	mutex_lock(mmac_lock);
	ptr = memory_mult_alloc_pages(pages);
	if ( ptr == NULL )
		printf("memory_alloc_pages(%d):Out of memory! Returning NULL\n");
	mutex_unlock(mmac_lock);
	return ptr;
}

void memory_init()
{
	mutex_init(mmac_lock);
	mutex_lock(mmac_lock);
	klog(LOG_INFO,"MEM","Initialising and populating memory...\n");
	mem_lastpage = pa_first_frame() * 0x1000;
	printf("mem_lastpage = 0x%X (%d)\n",mem_lastpage,mem_lastpage);
	mutex_unlock(mmac_lock);
}