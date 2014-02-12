#include <log.h>
#include <stdarg.h>
#include <stdio.h>
#include <video.h>

int klog_mask = LOG_DEBUG;
static int background = 0;
void klog(int mode, const char *title, const char *fmt, ...)
{
	#ifdef X86
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
				video_setattributetext(background,0x9);
				break;
			case LOG_DEBUG:
				video_setattributetext(background,0x8);///was 0x8;
				break;
			default:
				video_setattributetext(background,0x7);
		}
		printf("[%s]:", title);
		va_list args;
		int rv;
		va_start(args, fmt);
		rv = vprintf(fmt, args);
		va_end(args);
		video_resetattributetext();
	}
	#endif
}

void klog_setmask(int mode)
{
	klog_mask = mode;
}