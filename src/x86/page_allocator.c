#include <types.h>
#include <x86/placement_malloc.h>
#include <string.h>
#ifdef DEBUG
#include <stdio.h>
#endif
uint32_t *frame; //Pointer to first frame, first index.
uint32_t frame_amount; //How many frames CAN there be?
uint32_t mem_end = 0x1000000; //Where does memory end. Default's to 16mb of ram
uint32_t mem_end_aligned; //Where does memory end, page aligned.

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFEST_FROM_BIT(a) (a%(8*4))

void pa_set_frame(uint32_t address)
{
	uint32_t frame_addr = address / 0x1000;
	uint32_t index = INDEX_FROM_BIT(frame_addr);
	uint32_t offset = OFFEST_FROM_BIT(frame_addr);
	frame[index] |= (0x1 << offset);
}
void pa_clear_frame(uint32_t address)
{
	uint32_t frame_addr = address / 0x1000;
	uint32_t index = INDEX_FROM_BIT(frame_addr);
	uint32_t offset = OFFEST_FROM_BIT(frame_addr);
	frame[index] &= ~(0x1 << offset);
}
uint32_t pa_test_frame(uint32_t address)
{
	uint32_t frame_addr = address / 0x1000;
	uint32_t index = INDEX_FROM_BIT(frame_addr);
	uint32_t offset = OFFEST_FROM_BIT(frame_addr);
	return frame[index] & (0x1 << offset);
}

void init_page_allocator()
{
	mem_end_aligned = (mem_end & 0xFFFFF000);
	#ifdef DEBUG
	printf("Page Allocator:\n");
	int amm_alloc_mb = ((mem_end_aligned/1024)/1024);
	printf("---> Allocating for 0x%X (~%d MB) of pages\n",mem_end_aligned,amm_alloc_mb);
	#endif
	frame_amount = mem_end_aligned / 0x1000;
	frame = (uint32_t*)placement_kmalloc(INDEX_FROM_BIT(frame_amount));
	memset(frame, 0, INDEX_FROM_BIT(frame_amount)); //Clear frame
	#ifdef DEBUG
	printf("---> Allocatable frames: 0x%X (%d), starting at 0x%X\n",frame_amount,frame_amount,frame);
	#endif
}