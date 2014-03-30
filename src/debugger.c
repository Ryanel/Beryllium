#include <types.h>
#include <video.h>
#include <terminal.h>
#include <kstat.h>
extern const char *statusBarTitle;
const char *oldtitle;
void enter_debugger()
{
	kernel_kdebug = true;
	oldtitle = statusBarTitle;
	terminal_set_statusbar("Beryllium - SMM Kernel Level Debugger");
	terminal_clear();
	printf("Beryllium Kernel Debugger\n");
	printf("________________________________________________________________________________");
}