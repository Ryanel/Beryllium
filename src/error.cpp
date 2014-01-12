#include <log.h>
#include <error.h>
#include <terminal.h>
void panic(const char* reason)
{
	terminal_set_statusbar("   === PANIC ===   ");
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