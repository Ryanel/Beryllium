#include <log.h>
#include <stdarg.h>
#include <stdio.h>
#include <low_textmode.h>
void klog(int mode, const char *title, const char *fmt, ...)
{
	switch (mode)
	{
		case LOG_PANIC:
			textmode_setcolor(0,0xC);
			break;
		case LOG_SEVERE:
			textmode_setcolor(0,0xC);
			break;
		case LOG_ERROR:
			textmode_setcolor(0,0xC);
			break;
		case LOG_WARN:
			textmode_setcolor(0,0xC);
			break;
		case LOG_INFO:
			textmode_setcolor(0,0xC);
			break;
		case LOG_DEBUG:
			textmode_setcolor(0,0xC);
			break;
		default:
			textmode_setcolor(0,15);
	}
	printf("[%s]:", title);
	va_list args;
	int rv;
	va_start(args, fmt);
	rv = vprintf(fmt, args);
	va_end(args);
	textmode_resetcolor();
}