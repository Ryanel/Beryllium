#ifndef KSTDLIB_H
#define KSTDLIB_H
#include <types.h>
uint32_t kmalloc(uint32_t sz);
uint32_t kmalloc_a(uint32_t sz, int align);
uint32_t kmalloc_ap(uint32_t sz, int align, uint32_t *phys);
#endif