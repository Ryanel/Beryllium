#include <log.h>
#include <error.h>
#include <terminal.h>
#include <stdio.h>
#define PANIC_MSG_BAR "   ==================================PANIC===================================   "
void panic(const char* reason)
{
	terminal_set_statusbar(PANIC_MSG_BAR);
	textmode_setcolor(0,0xC);
	printf(PANIC_MSG_BAR);
	textmode_resetcolor();
	klog(LOG_PANIC,"PANIC",reason);
	klog(LOG_PANIC,"PANIC","Debug output:\n");
	klog(LOG_PANIC,"PANIC","Debug register output unimplemented\n");
	halt();
}

void halt()
{
	klog(LOG_INFO,"KERN","Halting...\n");
	asm("hlt");
}