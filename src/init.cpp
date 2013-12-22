#include <low_textmode.h>

extern "C" void kernel_init()
{
	textmode_setcursor(79,0);
	textmode_write(78,0,':');
	textmode_write(79,0,')');
}
