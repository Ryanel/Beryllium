#ifndef X86_H
#define X86_H
void init_x86();
void gdt_setup();
int idt_setup();
int isrs_setup();
void irq_install();
#endif
