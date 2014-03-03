#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <types.h>
typedef struct {
	uint8_t *memory_start;
	uint8_t *memory_end; //Does not have to be assigned.
	int width;
	int height;
	int depth;
	int pitch;
	int pixwidth;

} framebuffer_t;

void fb_putpixel(framebuffer_t fb, int x,int y, int color);

#endif