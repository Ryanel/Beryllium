//Low Level Display Drivers
#ifndef TERMINAL_H
#define TERMINAL_H
#include <types.h>
#include <low_textmode.h>

void printc(unsigned char c);
void print(const char *c);
void scroll();
void terminal_init();
#endif
