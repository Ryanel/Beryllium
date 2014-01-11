#ifndef LOG_H
#define LOG_H
#define LOG_PANIC 0
#define LOG_SEVERE 1
#define LOG_ERROR 2
#define LOG_WARN 3
#define LOG_INFO 4
#define LOG_DEBUG 5
void klog(int mode, const char *title, const char *fmt, ...);
#endif
