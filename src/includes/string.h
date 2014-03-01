#ifndef STRING_H
#define STRING_H
#include <types.h>
int strcmp (const char *str1,const char *str2);
//unsigned int strlen(unsigned char *str);
void *memcpy(void *dest,const void *src,size_t n);
void *memset(void *dest,int val,size_t n);
void memmove(void *dest, const void *src, size_t n);
#endif
