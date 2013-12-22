#include "includes/low_textmode.h"

extern "C" void kernel_init()
{
	textmode_setcursor(80,24);
}
