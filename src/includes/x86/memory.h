#ifndef X86_MEMORY_H
#define X86_MEMORY_H
#include <types.h>
//void* memory_mult_alloc_pages(int pages);
void* memory_alloc_pages(int pages);
void memory_init();
void *sbrk(size_t amount);
#endif
