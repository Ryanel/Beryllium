#ifndef STDIO_H
#define STDIO_H
#include <stdarg.h>
/// wikipedia.org/printf
/**
Prints formated text. Can print to any video device implemented in video.c
**/
int printf(const char *fmt, ...);
/**
Prints formated text.
**/
int vprintf(const char *fmt, va_list args);
#endif
