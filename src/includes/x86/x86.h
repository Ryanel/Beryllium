#ifndef X86_H
#define X86_H

#include <x86/gdt.h>
#include <x86/idt.h>
void init_x86();
void gdt_setup();
int idt_setup();
int isrs_setup();
void irq_install();
#endif
