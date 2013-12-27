#ifndef X86_H
#define X86_H

#include <gdt.h>
#include <idt.h>
void init_x86();
void gdt_setup();
void idt_setup();

#endif
