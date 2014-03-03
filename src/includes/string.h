#ifndef STRING_H
#define STRING_H
#include <types.h>
///Compares two strings
int strcmp (const char *str1,const char *str2);
//unsigned int strlen(unsigned char *str);
///Copies src(n) to dest
void *memcpy(void *dest,const void *src,size_t n);
///Sets dest to val n times
void *memset(void *dest,int val,size_t n);
///Moves n src to dest
void memmove(void *dest, const void *src, size_t n);
#endif
