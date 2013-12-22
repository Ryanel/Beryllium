#ifndef PORTS_H
#define PORTS_H
#include <types.h>
void outb(uint16_t port, uint8_t value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}
#endif
