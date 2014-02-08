#include <x86/ports.h>
#include <drivers/bga.h>
#include <stdio.h>

bool bga_isinit = false;
void bga_writeregister(uint16_t index, uint16_t value)
{
	outw(BGA_VBE_DISPI_IOPORT_INDEX, index);
	outw(BGA_VBE_DISPI_IOPORT_DATA, value);
}
uint16_t bga_readregister(uint16_t index)
{		
	outw(BGA_VBE_DISPI_IOPORT_INDEX, index);
    return inw(BGA_VBE_DISPI_IOPORT_DATA);
}

int bga_isavalable()
{
	uint16_t ver = bga_readregister(BGA_VBE_DISPI_INDEX_ID);
	if(ver < 0xB0C2)
	{
		return 0;
	}
	if(ver > 0xB0C5)
	{
		return 0;
	}
	return 1;
}

int bga_init()
{
	if(!bga_isavalable())
	{
		return 1;
	}
	bga_setmode(640,480,0x20,1,1);
	bga_isinit = true;
	return 0;
}

void bga_setmode(unsigned int width, unsigned int height, unsigned int bitdepth, int linearframebuffer, int clearvideomemory)
{
	bga_writeregister(BGA_VBE_DISPI_INDEX_ENABLE, BGA_VBE_DISPI_DISABLED);
	bga_writeregister(BGA_VBE_DISPI_INDEX_XRES, width);
	bga_writeregister(BGA_VBE_DISPI_INDEX_YRES, height);
	bga_writeregister(BGA_VBE_DISPI_INDEX_BPP, bitdepth);
	bga_writeregister(BGA_VBE_DISPI_INDEX_ENABLE, BGA_VBE_DISPI_ENABLED |
	(linearframebuffer ? BGA_VBE_DISPI_LFB_ENABLED : 0) |
	(clearvideomemory ? 0 : BGA_VBE_DISPI_NOCLEARMEM));
}