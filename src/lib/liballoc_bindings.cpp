#include <x86/low_paging.h>
#include <lib/liballoc.h>
/** This function is supposed to lock the memory data structures. It
 * could be as simple as disabling interrupts or acquiring a spinlock.
 * It's up to you to decide. 
 *
 * \return 0 if the lock was acquired successfully. Anything else is
 * failure.
 */
int liballoc_lock()
{
	asm("cli");
	return 0;
}

/** This function unlocks what was previously locked by the liballoc_lock
 * function.  If it disabled interrupts, it enables interrupts. If it
 * had acquiried a spinlock, it releases the spinlock. etc.
 *
 * \return 0 if the lock was successfully released.
 */
 int liballoc_unlock()
{
	asm("sti");
	return 0;
}

/** This is the hook into the local system which allocates pages. It
 * accepts an integer parameter which is the number of pages
 * required.  The page size was set up in the liballoc_init function.
 *
 * \return NULL if the pages were not allocated.
 * \return A pointer to the allocated memory.
 */
page_t *get_page(uint32_t address, int make, page_directory_t *dir);
void paging_alloc_frame(page_t *page, int is_kernel, int is_writeable);
extern page_directory_t* kernel_directory;
uint32_t first_frame();
void* liballoc_alloc(int page)
{
	int first_free_frame = first_frame();

	int i = 0;
	while (i < page)
	{
		// Kernel code is readable but not writeable from userspace.
		paging_alloc_frame( get_page(first_free_frame + (i * 0x1000), 1, kernel_directory), 0, 0);
		i ++;
	}
	return 0;
}

/** This frees previously allocated memory. The void* parameter passed
 * to the function is the exact same value returned from a previous
 * liballoc_alloc call.
 *
 * The integer value is the number of pages to free.
 *
 * \return 0 if the memory was successfully freed.
 */
int liballoc_free(void*,int)
{
	return 1;
}

       