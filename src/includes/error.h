#ifndef ERROR_H
#define ERROR_H
#include <x86/isr.h>
void panic(const char* reason);
void halt();
void halt_regs(registers_t* regs);
#endif