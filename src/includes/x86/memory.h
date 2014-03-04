#ifndef X86_MEMORY_H
#define X86_MEMORY_H
#include <types.h>
#include <lib/ordered_array.h>
//void* memory_mult_alloc_pages(int pages);
void* memory_alloc_pages(int pages);
void memory_init();
void *sbrk(size_t amount);

typedef struct
{
	uint32_t magic;  /// Magic number, used for error checking and identification.
	uint8_t is_hole; /// 1 if this is a hole, 0 if this is a block.
	uint32_t size;   /// Size of the block, including this and the footer.
} k_memory_header_t;

typedef struct
{
	uint32_t magic;     /// Magic number, same as in k_memory_header_t.
	k_memory_header_t *header; /// Pointer to the block header.
} k_memory_footer_t;

#define HEAP_MAGIC			0xC0DECAFE
/// A heap
typedef struct
{
	ordered_array_t index;
	uint32_t start_address; /// The start of our allocated space.
	uint32_t end_address;   /// The end of our allocated space. May be expanded up to max_address.
	uint32_t max_address;   /// The maximum address the heap can be expanded to.
	uint8_t supervisor;     /// Should extra pages requested by us be mapped as supervisor-only?
	uint8_t readonly;       /// Should extra pages requested by us be mapped as read-only?
} heap_t;
///Creates a heap
heap_t *create_heap(uint32_t start, uint32_t end_addr, uint32_t max, uint8_t supervisor, uint8_t readonly);
#endif
