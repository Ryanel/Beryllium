#include "includes/low_textmode.h"
#include "includes/types.h"
#include "includes/ports.h"
uint16_t *video_memory=(uint16_t*)0xB8000;
void textmode_setcursor(int x,int y)
{
	uint16_t location = y * 80 + x;
	outb(0x3D4, 14);
	outb(0x3D5, location >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, location);
}
void textmode_write(int x,int y,uint8_t data)
{
	uint16_t *write_to = video_memory + (y * 80 + x);
	*write_to = data;
}

void textmode_write_color(int x,int y,uint8_t data)
{
	uint16_t *write_to = video_memory + ((y * 80 + x ) + 1);
	*write_to = data;
}

uint8_t textmode_read(int x,int y)
{
	return 0;
}
void textmode_scroll()
{
}
