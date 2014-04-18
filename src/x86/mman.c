/**
 * Beryllium Regioned Memory Manager
 * Written by Corwin McKnight
 * WARNING: This file deals with memory but CANNOT use allocation. Make sure things are dynamic ONLY if we got the heap up
**/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#define PAGE_SIZE 0x1000
#define DEBUG 1
typedef struct mman_region {
	struct 		mman_region  * prev;
	struct 		mman_region * next;
	uint8_t		used;				// Is this used?
	uint8_t 	type;				// 0 - System, 1 - Heap, 2 - Usercode, 3 - MMIO, 4 - Reserved, 5 - Reserved (extra)
	uint8_t 	flags;				// VK-----I (Virtual, Kernel, RESV * 5, Initialised)
	uintptr_t 	start;				// Must be page aligned
	size_t 		size;
} mman_region_t;

/// 0xFF preallocated memory regions until we can get a bootstrap memory system online.
mman_region_t mman_prealloc[0xFF];
mman_region_t * mman_region;
uint8_t mman_prealloc_ptr = 0;

/// The status of the memory manager (to deny memory allocations and etc)
uint8_t mman_status = 0;

/**
 * Signal the memory manager about a new region.
**/
int memsig(uintptr_t start, size_t size, uint8_t type, uint8_t flags)
{
	if(start == (uintptr_t)NULL)
	{
		printf("memsig: passed null address -- not modifying anything\n");
		return 1;
	}
	if (mman_status == 0) //We can't do anything if memory_init hasn't been called!
	{
		return 2;
	}
	mman_region_t * region;
	if (mman_status == 1) //memory_init called, but no heap defined!
	{
		region = &mman_prealloc[mman_prealloc_ptr++];
	}
	else
	{
		region = malloc(sizeof(mman_region_t));
	}
	region->start = start;
	region->size  = size;
	region->type  = type;
	region->flags = flags;
	region->used  = 0x1;
	if(region->type == 0x1 && mman_status == 0x1)
	{
		mman_status = 0x2; //We got a heap!
	}
	// Its detached from the main list, so we need to find the next free item
	mman_region_t * parent_ptr = mman_region;
	while (1)
	{
		if(parent_ptr == NULL) // On first memsig
		{
			parent_ptr = region;
			region->next = NULL;
			break;
		}
		else if(parent_ptr->next == NULL)
		{
			parent_ptr->next = region;
			region->prev     = parent_ptr;
			region->next     = NULL;
			break;
		}
		else
		{
			parent_ptr = parent_ptr->next;
		}
	}
	return 0;
}

/**
 * Safely modify the memory region(to prevent boundery fails)
**/
int memresize(mman_region_t * region, size_t size)
{
	return 0;
}

/**
 * Initialize the memory manager. Refrain from using memsig & malloc
**/
void memman_init()
{
	printf("Starting memory manager...\n");
	memset(mman_prealloc,0,sizeof(mman_region_t) * 0xFF);
	mman_region = &mman_prealloc[0]; //Assign it to the first preallocated block
	mman_status = 1;
}