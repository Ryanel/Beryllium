#include <log.h>
#include <stdarg.h>
#include <stdio.h>
#include <beryllium/video.h>
#include <beryllium/timer.h>
int klog_mask = LOG_DEBUG;
static int background = 0;
#ifdef LOG_SERIAL
	#define KPRINTF serial_printf
#else
	#define KPRINTF printf
#endif
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
				video_setattributetext(background,0x7);///was 0x8;
				break;
			case LOG_OK:
				video_setattributetext(background,0xA);///was 0x8;
				break;
			case LOG_FAIL:
				video_setattributetext(background,0x4);///was 0x8;
				break;
			default:
				video_setattributetext(background,0x8);
		}
		#ifdef KLOG_TITLE_TIME
		KPRINTF("[%08d]:",timer_getHi());
		#else
		if(mode == LOG_OK)
		{
			KPRINTF("[OKAY]");
		}
		else if (mode == LOG_FAIL)
		{
			KPRINTF("[FAIL]");
		}
		else
		{
			KPRINTF("[%s]:",title);
		}
		#endif
		va_list args;
		int rv;
		va_start(args, fmt);
		#ifdef LOG_SERIAL
		rv = vprintf_serial(fmt, args);
		#else
		rv = vprintf(fmt,args);
		#endif
		va_end(args);
		video_resetattributetext();
	}
}

void klog_setmask(int mode)
{
	klog_mask = mode;
}
