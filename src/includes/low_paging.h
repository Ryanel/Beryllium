#ifndef PAGING_H
#define PAGING_H
#include <types.h>
#include <isr.h>
void paging_init();
void paging_fault(struct regs *regs);
#endif
