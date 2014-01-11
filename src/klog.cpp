#include <log.h>
#include <stdarg.h>
#include <stdio.h>
void klog(int mode, const char *title, const char *fmt, ...)
{
	printf("[%s]:", title);
	va_list args;
	int rv;
	va_start(args, fmt);
	rv = vprintf(fmt, args);
	va_end(args);
}