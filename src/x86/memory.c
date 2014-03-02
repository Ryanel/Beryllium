#include <x86/paging.h>
#include <log.h>
#include <x86/page_allocator.h>
#include <x86/placement_malloc.h>
//Allocates multiple pages
void* memory_mult_alloc_pages(int pages)
{
	int i, j;
	int block = -1;
  	for(i = 0; i < pa_frame_amount(); i++)
  	{
  		int pos = (i) % pa_frame_amount(); // Create a position based off of i.
		if ( (pos + pages) > pa_frame_amount() )
		{
			i += pages - 2;
			continue;	
		}
		block = pos;
		for(j = 0;j < pages; j++)
		{
			if ( pa_test_frame(INDEX_FROM_BIT(pos + j) ) ) 
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
		pa_set_frame(INDEX_FROM_BIT(block + i));

	//s_lastPage = block + pages;

	return (void*)((unsigned int)block * 0x1000 );
}