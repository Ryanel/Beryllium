#ifndef LOW_CPU_H
#define LOW_CPU_H

inline void cpu_disableinterrupts()
{
	asm("cli");
}
inline void cpu_enableinterrupts()
{
	asm("sti");
}

#endif
