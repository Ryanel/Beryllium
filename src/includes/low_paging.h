#ifndef PAGING_H
#define PAGING_H
#include <types.h>
#include <isr.h>
void paging_init();
void switch_page_directory(page_directory_t*);
void page_fault(registers_t regs);
#endif