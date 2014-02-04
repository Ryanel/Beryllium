//Low Level Display Drivers
#ifndef TERMINAL_H
#define TERMINAL_H
#include <types.h>
#include <drivers/low_textmode.h>
#define STATUS_BAR_ATTRIBUTE 0x1F
void printc(unsigned char c);
void print(const char *c);
void scroll();
void terminal_init();

void terminal_clear_statusbar();

void terminal_set_statusbar(const char *c);
#endif
