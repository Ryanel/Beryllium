#ifndef KSTDLIB_H
#define KSTDLIB_H
uint32_t kmalloc(uint32_t sz);
uint32_t kmalloc(uint32_t sz, int align);
uint32_t kmalloc(uint32_t sz, int align, uint32_t *phys);
#endif