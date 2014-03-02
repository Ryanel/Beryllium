#ifndef PAGE_ALLOCATOR_H
#define PAGE_ALLOCATOR_H
#include <types.h>
#include <x86/paging.h>
#define INDEX_FROM_BIT(b) (b / 0x20)
#define OFFSET_FROM_BIT(b) (b % 0x20)
void init_page_allocator();
void pa_set_frame(uint32_t address);
void pa_clear_frame(uint32_t address);
uint32_t pa_test_frame(uint32_t address);
uint32_t pa_first_frame();
uint32_t pa_frame_amount();
void pa_alloc_frame(page_t *page, int kernel, int rw);
void pa_free_frame(page_t *page);
#endif