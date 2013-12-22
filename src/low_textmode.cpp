#include <low_textmode.h>
#include <types.h>
#include <ports.h>
uint8_t attributeByte = (0 << 4) | (15  & 0x0F);
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
	uint16_t *video_memory=(uint16_t*)0xB8000;
	uint16_t attribute = attributeByte << 8;
	uint16_t *write_to;
	write_to = video_memory + ((y * 80) + x);
	*write_to = data | attribute;
}
uint8_t textmode_read(int x,int y)
{
	return 0;
}
void textmode_scroll()
{
	uint16_t *video_memory=(uint16_t*)0xB8000;
	uint16_t blank = 0x20  | (attributeByte << 8);
	int i;
	for (i = 0*80; i < 24*80; i++)
	{
		video_memory[i] = video_memory[i+80];
	}

	for (i = 24*80; i < 25*80; i++)
	{
		video_memory[i] = blank;
	}
}
