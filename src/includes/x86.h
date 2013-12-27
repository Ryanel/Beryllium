#ifndef X86_H
#define X86_H

void init_x86();

struct gdt_entry
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));
///A pointer to the GDT; passsed to lgdt
struct gdt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));
extern "C" void gdt_flush();
void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);

#endif
