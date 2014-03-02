#ifndef STDLIB_H
#define STDLIB_H
void     *malloc(size_t);				//< The standard function.
void*     realloc(void *p, size_t);		//< The standard function.
void     *calloc(size_t, size_t);		//< The standard function.
void      free(void *);					//< The standard function.
#endif