/**
Manages the memory

Hooks into src/memory.c
**/
#include <x86/paging.h>
#include <x86/page_allocator.h>
#include <x86/lib/placement_malloc.h>
#include <x86/memory.h>
#include <types.h>
#include <stdio.h>
#include <log.h>
#include <mutex.h>

///Page Directory
extern page_directory_t *kernel_directory;
///How many frames are free? Set to a default 0x10000000 to allow allocation BEFORE it's set. Because I am lazy.
int mem_pagefree = 0x10000000;
/**
The last page that was allocated. Used to speed up bitmap searches for free frames
Set by memory_init(); Running memory_mult_alloc_pages is possible without this set, but it takes a couple SECONDS on the first allocation,
as it must search every possible frame (about ~0x150000 frames). This is set to start the search at the last allocated frame (0x150000) so we
can search from there up.
**/
int mem_lastpage = 0;
///Kernel memory allocation mutex (boolean semaphore). This is NOT the lock on memory, only for the memory_alloc allocators.
mutex_t *mmac_lock;
/// The kernel's heap
heap_t *kheap; 
/**
The kernel reserved area is exactly 0x1000 bytes of memory. It is a sort of "shield" against the heap's conventional memory.
This is filled completely with 0xFF, and in the event of an EMERGENCY it can be written to to store up to 4kb of debug information.
This memory is not for conventional use; infact it is never used in the current implementation. It is just a shield.
**/
void* kernel_reserved_area;
/**
Allocates multple pages and returns a pointer to the first page.
This function is not meant to be directly called, as this is not locked by the mutex mmac_lock.
Please use memory_alloc_pages() instead.
**/
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
/**
Allocates multple pages and returns a pointer to the first page.

This function IS meant to be directly called, as this is locked by the mutex mmac_lock.
Please don't call memory_mult_alloc_pages() instead.
**/
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
/**
De-allocates pages
**/	
void memory_dealloc_pages(int pages)
{
	if(pages <= 0)
	{
		return;
	}

	mutex_lock(mmac_lock);
	int i = pages;
	int blk = mem_lastpage;
	while(i != 0)
	{
		pa_free_frame(paging_get_page(blk--,1, kernel_directory));
		i--;
	}
	mutex_unlock(mmac_lock);
}
/**
Initializes Memory

Specificallly, it locks memory, initialises mem_lastpage, and allocates the kernel_reserved_area.
**/
void memory_init()
{
	mutex_init(mmac_lock);
	mutex_lock(mmac_lock);
	klog(LOG_INFO,"MEM","Initialising and populating memory...\n");
	mem_lastpage = pa_first_frame() * 0x1000;
	klog(LOG_DEBUG,"MEM","Marked 0x%X (%d) frames as dirty\n",mem_lastpage,mem_lastpage);
	mutex_unlock(mmac_lock);
	klog(LOG_DEBUG,"MEM","Allocating Kernel Reserved Area...\n");
	kernel_reserved_area = memory_alloc_pages(1);
	klog(LOG_DEBUG,"MEM","Creating heap...!\n");
	kheap = create_heap(KHEAP_START, KHEAP_START+KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);
	klog(LOG_INFO,"MEM","Done allocating initial memory!\n");
}
/**
Allocates amount bytes, and returns a pointer to the beginning of the allocated amount.
**/
void *sbrk(size_t amount)
{
	if(amount == 0)
	{
		return (void*)((unsigned int)mem_lastpage);
	}
	uint32_t actual_amount = (amount / 0x1000) +1;
	printf("sbrk: allocating %d pages to cover 0x%X bytes\n",actual_amount,amount);
	void *tmp = memory_alloc_pages(actual_amount);
	return tmp;
}
/**
Deallocates memory(pages) from end_data_segment forward.
\todo Implement brk
**/
int brk(void *end_data_segment)
{
	return 0;
}
void memory_parse_grub()
{

}

static sint32_t find_smallest_hole(uint32_t size, uint8_t page_align, heap_t *heap)
{
	// Find the smallest hole that will fit.
	uint8_t iterator = 0;
	while (iterator < heap->index.size)
	{
		k_memory_header_t *header = (k_memory_header_t *)lookup_ordered_array(iterator, &heap->index);
		// If the user has requested the memory be page-aligned
		if (page_align > 0)
		{
			// Page-align the starting point of this header.
			uint32_t location = (uint32_t)header;
			sint32_t offset = 0;
			if (((location+sizeof(k_memory_header_t)) & 0xFFFFF000) != 0)
			offset = 0x1000 /* page size */  - (location+sizeof(k_memory_header_t))%0x1000;
			sint32_t hole_size = (sint32_t)header->size - offset;
			// Can we fit now?
			if (hole_size >= (sint32_t)size)
				break;
		}
		else if (header->size >= size)
		break;
		iterator++;
	}
	// Why did the loop exit?
	if (iterator == heap->index.size)
		return -1; // We got to the end and didn't find anything.
	else
		return iterator;
}

static sint8_t header_t_less_than(void*a, void *b)
{
	return (((k_memory_header_t*)a)->size < ((k_memory_header_t*)b)->size)?1:0;
}

heap_t *create_heap(uint32_t start, uint32_t end_addr, uint32_t max, uint8_t supervisor, uint8_t readonly)
{
	heap_t *heap = (heap_t*)placement_kmalloc(sizeof(heap_t));

	// All our assumptions are made on startAddress and endAddress being page-aligned.
	if(!(start%0x1000 == 0))
	{
		printf("Woah! You can't create a heap at unaligned address 0x%X!\n",start);
	}
	if(!(end_addr%0x1000 == 0))
	{
		printf("Woah! You can't create a heap ending at unaligned address 0x%X!\n",end_addr);
	}

	// Initialise the index.
	heap->index = place_ordered_array( (void*)start, HEAP_INDEX_SIZE, &header_t_less_than);

	// Shift the start address forward to resemble where we can start putting data.
	start += sizeof(type_t)*HEAP_INDEX_SIZE;

	// Make sure the start address is page-aligned.
	if ((start & 0xFFFFF000) != 0)
	{
		start &= 0xFFFFF000;
		start += 0x1000;
	}
	// Write the start, end and max addresses into the heap structure.
	heap->start_address = start;
	heap->end_address = end_addr;
	heap->max_address = max;
	heap->supervisor = supervisor;
	heap->readonly = readonly;

	// We start off with one large hole in the index.
	k_memory_header_t *hole = (k_memory_header_t *)start;
	hole->size = end_addr-start;
	hole->magic = HEAP_MAGIC;
	hole->is_hole = 1;
	insert_ordered_array((void*)hole, &heap->index);

	return heap;
}


static void expand(uint32_t new_size, heap_t *heap)
{
    // Sanity check.
    if(!(new_size > heap->end_address - heap->start_address))
    {
    	printf("Size is too small!");
    	return;
    }

    // Get the nearest following page boundary.
    if ((new_size&0xFFFFF000) != 0)
    {
        new_size &= 0xFFFFF000;
        new_size += 0x1000;
    }

    // Make sure we are not overreaching ourselves.
    if(!(heap->start_address+new_size <= heap->max_address))
    {
    	printf("Attempted to allocate too much for heap!\n");
    	return;
    }

    // This should always be on a page boundary.
    uint32_t old_size = heap->end_address-heap->start_address;

    uint32_t i = old_size;
    while (i < new_size)
    {
        pa_alloc_frame( paging_get_page(heap->start_address+i, 1, kernel_directory), (heap->supervisor)?1:0, (heap->readonly)?0:1);
        i += 0x1000 /* page size */;
    }
    heap->end_address = heap->start_address+new_size;
}

static uint32_t contract(uint32_t new_size, heap_t *heap)
{
    // Sanity check.
    if(!(new_size < heap->end_address-heap->start_address))
    {
    	printf("Contract failed! new_size < heap->end_address-heap->start_address!");
    }

    // Get the nearest following page boundary.
    if (new_size&0x1000)
    {
        new_size &= 0x1000;
        new_size += 0x1000;
    }

    // Don't contract too far!
    if (new_size < HEAP_MIN_SIZE)
        new_size = HEAP_MIN_SIZE;

    uint32_t old_size = heap->end_address-heap->start_address;
    uint32_t i = old_size - 0x1000;
    while (new_size < i)
    {
        pa_free_frame(paging_get_page(heap->start_address+i, 0, kernel_directory));
        i -= 0x1000;
    }

    heap->end_address = heap->start_address + new_size;
    return new_size;
}

void *alloc(uint32_t size, uint8_t page_align, heap_t *heap)
{

    // Make sure we take the size of header/footer into account.
    uint32_t new_size = size + sizeof(k_memory_header_t) + sizeof(k_memory_footer_t);
    // Find the smallest hole that will fit.
    sint32_t iterator = find_smallest_hole(new_size, page_align, heap);

    if (iterator == -1) // If we didn't find a suitable hole
    {
        // Save some previous data.
        uint32_t old_length = heap->end_address - heap->start_address;
        uint32_t old_end_address = heap->end_address;

        // We need to allocate some more space.
        expand(old_length+new_size, heap);
        uint32_t new_length = heap->end_address-heap->start_address;

        // Find the endmost header. (Not endmost in size, but in location).
        iterator = 0;
        // Vars to hold the index of, and value of, the endmost header found so far.
        uint32_t idx = -1; uint32_t value = 0x0;
        while (iterator < heap->index.size)
        {
            uint32_t tmp = (uint32_t)lookup_ordered_array(iterator, &heap->index);
            if (tmp > value)
            {
                value = tmp;
                idx = iterator;
            }
            iterator++;
        }

        // If we didn't find ANY headers, we need to add one.
        if (idx == -1)
        {
            k_memory_header_t *header = (k_memory_header_t *)old_end_address;
            header->magic = HEAP_MAGIC;
            header->size = new_length - old_length;
            header->is_hole = 1;
            k_memory_footer_t *footer = (k_memory_footer_t *) (old_end_address + header->size - sizeof(k_memory_footer_t));
            footer->magic = HEAP_MAGIC;
            footer->header = header;
            insert_ordered_array((void*)header, &heap->index);
        }
        else
        {
            // The last header needs adjusting.
            k_memory_header_t *header = lookup_ordered_array(idx, &heap->index);
            header->size += new_length - old_length;
            // Rewrite the footer.
            k_memory_footer_t *footer = (k_memory_footer_t *) ( (uint32_t)header + header->size - sizeof(k_memory_footer_t) );
            footer->header = header;
            footer->magic = HEAP_MAGIC;
        }
        // We now have enough space. Recurse, and call the function again.
        return alloc(size, page_align, heap);
    }

    k_memory_header_t *orig_hole_header = (k_memory_header_t *)lookup_ordered_array(iterator, &heap->index);
    uint32_t orig_hole_pos = (uint32_t)orig_hole_header;
    uint32_t orig_hole_size = orig_hole_header->size;
    // Here we work out if we should split the hole we found into two parts.
    // Is the original hole size - requested hole size less than the overhead for adding a new hole?
    if (orig_hole_size-new_size < sizeof(k_memory_header_t)+sizeof(k_memory_footer_t))
    {
        // Then just increase the requested size to the size of the hole we found.
        size += orig_hole_size-new_size;
        new_size = orig_hole_size;
    }

    // If we need to page-align the data, do it now and make a new hole in front of our block.
    if (page_align && orig_hole_pos&0xFFFFF000)
    {
        uint32_t new_location   = orig_hole_pos + 0x1000 /* page size */ - (orig_hole_pos&0xFFF) - sizeof(k_memory_header_t);
        k_memory_header_t *hole_header = (k_memory_header_t *)orig_hole_pos;
        hole_header->size     = 0x1000 /* page size */ - (orig_hole_pos&0xFFF) - sizeof(k_memory_header_t);
        hole_header->magic    = HEAP_MAGIC;
        hole_header->is_hole  = 1;
        k_memory_footer_t *hole_footer = (k_memory_footer_t *) ( (uint32_t)new_location - sizeof(k_memory_footer_t) );
        hole_footer->magic    = HEAP_MAGIC;
        hole_footer->header   = hole_header;
        orig_hole_pos         = new_location;
        orig_hole_size        = orig_hole_size - hole_header->size;
    }
    else
    {
        // Else we don't need this hole any more, delete it from the index.
        remove_ordered_array(iterator, &heap->index);
    }

    // Overwrite the original header...
    k_memory_header_t *block_header  = (k_memory_header_t *)orig_hole_pos;
    block_header->magic     = HEAP_MAGIC;
    block_header->is_hole   = 0;
    block_header->size      = new_size;
    // ...And the footer
    k_memory_footer_t *block_footer  = (k_memory_footer_t *) (orig_hole_pos + sizeof(k_memory_header_t) + size);
    block_footer->magic     = HEAP_MAGIC;
    block_footer->header    = block_header;

    // We may need to write a new hole after the allocated block.
    // We do this only if the new hole would have positive size...
    if (orig_hole_size - new_size > 0)
    {
        k_memory_header_t *hole_header = (k_memory_header_t *) (orig_hole_pos + sizeof(k_memory_header_t) + size + sizeof(k_memory_footer_t));
        hole_header->magic    = HEAP_MAGIC;
        hole_header->is_hole  = 1;
        hole_header->size     = orig_hole_size - new_size;
        k_memory_footer_t *hole_footer = (k_memory_footer_t *) ( (uint32_t)hole_header + orig_hole_size - new_size - sizeof(k_memory_footer_t) );
        if ((uint32_t)hole_footer < heap->end_address)
        {
            hole_footer->magic = HEAP_MAGIC;
            hole_footer->header = hole_header;
        }
        // Put the new hole in the index;
        insert_ordered_array((void*)hole_header, &heap->index);
    }
    
    // ...And we're done!
    return (void *) ( (uint32_t)block_header+sizeof(k_memory_header_t) );
}

void free(void *p, heap_t *heap)
{
    // Exit gracefully for null pointers.
    if (p == 0)
        return;

    // Get the header and footer associated with this pointer.
    k_memory_header_t *header = (k_memory_header_t*) ( (uint32_t)p - sizeof(k_memory_header_t) );
    k_memory_footer_t *footer = (k_memory_footer_t*) ( (uint32_t)header + header->size - sizeof(k_memory_footer_t) );

    // Sanity checks.
    if(!(header->magic == HEAP_MAGIC))
    {
    	printf("Free: bad block!\n");
    }

    // Make us a hole.
    header->is_hole = 1;

    // Do we want to add this header into the 'free holes' index?
    char do_add = 1;

    // Unify left
    // If the thing immediately to the left of us is a footer...
    k_memory_footer_t *test_footer = (k_memory_footer_t*) ( (uint32_t)header - sizeof(k_memory_footer_t) );
    if (test_footer->magic == HEAP_MAGIC &&
        test_footer->header->is_hole == 1)
    {
        uint32_t cache_size = header->size; // Cache our current size.
        header = test_footer->header;     // Rewrite our header with the new one.
        footer->header = header;          // Rewrite our footer to point to the new header.
        header->size += cache_size;       // Change the size.
        do_add = 0;                       // Since this header is already in the index, we don't want to add it again.
    }

    // Unify right
    // If the thing immediately to the right of us is a header...
    k_memory_header_t *test_header = (k_memory_header_t*) ( (uint32_t)footer + sizeof(k_memory_footer_t) );
    if (test_header->magic == HEAP_MAGIC &&
        test_header->is_hole)
    {
        header->size += test_header->size; // Increase our size.
        test_footer = (k_memory_footer_t*) ( (uint32_t)test_header + // Rewrite it's footer to point to our header.
                                    test_header->size - sizeof(k_memory_footer_t) );
        footer = test_footer;
        // Find and remove this header from the index.
        uint32_t iterator = 0;
        while ( (iterator < heap->index.size) &&
                (lookup_ordered_array(iterator, &heap->index) != (void*)test_header) )
            iterator++;

        // Make sure we actually found the item.
        if(!(iterator < heap->index.size))
        {
        	printf("Free: Item not found!\n");
        }
        // Remove it.
        remove_ordered_array(iterator, &heap->index);
    }

    // If the footer location is the end address, we can contract.
    if ( (uint32_t)footer+sizeof(k_memory_footer_t) == heap->end_address)
    {
        uint32_t old_length = heap->end_address-heap->start_address;
        uint32_t new_length = contract( (uint32_t)header - heap->start_address, heap);
        // Check how big we will be after resizing.
        if (header->size - (old_length-new_length) > 0)
        {
            // We will still exist, so resize us.
            header->size -= old_length-new_length;
            footer = (k_memory_footer_t*) ( (uint32_t)header + header->size - sizeof(k_memory_footer_t) );
            footer->magic = HEAP_MAGIC;
            footer->header = header;
        }
        else
        {
            // We will no longer exist :(. Remove us from the index.
            uint32_t iterator = 0;
            while ( (iterator < heap->index.size) &&
                    (lookup_ordered_array(iterator, &heap->index) != (void*)test_header) )
                iterator++;
            // If we didn't find ourselves, we have nothing to remove.
            if (iterator < heap->index.size)
                remove_ordered_array(iterator, &heap->index);
        }
    }

    // If required, add us to the index.
    if (do_add == 1)
        insert_ordered_array((void*)header, &heap->index);

}
