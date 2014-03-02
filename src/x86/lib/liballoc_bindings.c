#include <x86/memory.h>
#include <types.h>
int liballoc_lock()
{
	asm("cli");
	return 0;
}

extern int liballoc_unlock()
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
extern void* liballoc_alloc(int pages)
{
	if(pages == 0)
	{
		return NULL;
	}

	void * ret=memory_alloc_pages(pages);
	return ret;
}

/** This frees previously allocated memory. The void* parameter passed
 * to the function is the exact same value returned from a previous
 * liballoc_alloc call.
 *
 * The integer value is the number of pages to free.
 *
 * \return 0 if the memory was successfully freed.
 */
int liballoc_free(void* ptr,int pages)
{
	return 0;
}