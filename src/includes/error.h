#ifndef ERROR_H
#define ERROR_H
#ifdef X86
#include <x86/low/isr.h>
#endif
void panic(const char* reason);
void halt();
void halt_regs(registers_t* regs);
#endif
