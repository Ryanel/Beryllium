#include <log.h>
#include <stdarg.h>
#include <stdio.h>
#include <drivers/low_textmode.h>

int klog_mask = LOG_DEBUG;
static int background = 0;
void klog(int mode, const char *title, const char *fmt, ...)
{
	if( mode <= klog_mask)
	{
		switch (mode)
		{
			case LOG_PANIC:
				textmode_setcolor(background,0xC);
				break;
			case LOG_SEVERE:
				textmode_setcolor(background,0x6);
				break;
			case LOG_ERROR:
				textmode_setcolor(background,0x4);
				break;
			case LOG_WARN:
				textmode_setcolor(background,0xE);
				break;
			case LOG_INFO:
				textmode_setcolor(background,0x9);
				break;
			case LOG_DEBUG:
				textmode_setcolor(background,0x8);///was 0x8;
				break;
			default:
				textmode_setcolor(background,0x7);
		}
		printf("[%s]:", title);
		va_list args;
		int rv;
		va_start(args, fmt);
		rv = vprintf(fmt, args);
		va_end(args);
		textmode_resetcolor();
	}
}

void klog_setmask(int mode)
{
	klog_mask = mode;
}