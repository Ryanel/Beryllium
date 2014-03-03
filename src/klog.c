#include <log.h>
#include <stdarg.h>
#include <stdio.h>
#include <video.h>
#include <drivers/timer.h>
int klog_mask = LOG_DEBUG;
static int background = 0;
void klog(int mode, const char *title, const char *fmt, ...)
{
	if( mode <= klog_mask)
	{
		switch (mode)
		{
			case LOG_PANIC:
				video_setattributetext(background,0xC);
				break;
			case LOG_SEVERE:
				video_setattributetext(background,0x6);
				break;
			case LOG_ERROR:
				video_setattributetext(background,0x4);
				break;
			case LOG_WARN:
				video_setattributetext(background,0xE);
				break;
			case LOG_INFO:
				video_setattributetext(background,0xF);
				break;
			case LOG_DEBUG:
				video_setattributetext(background,0x8);///was 0x8;
				break;
			default:
				video_setattributetext(background,0x7);
		}
		#ifdef KLOG_TITLE_TIME
		printf("[%08d]:",timer_getHi());
		#else
		printf("[%s]:",title);
		#endif
		va_list args;
		int rv;
		va_start(args, fmt);
		rv = vprintf(fmt, args);
		va_end(args);
		video_resetattributetext();
	}
}

void klog_setmask(int mode)
{
	klog_mask = mode;
}