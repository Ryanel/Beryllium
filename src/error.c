#include <log.h>
#include <error.h>
#include <video.h>
#include <stdio.h>
#define PANIC_MSG_BAR "   ==================================PANIC===================================   "
void panic(const char* reason)
{
	//terminal_set_statusbar(PANIC_MSG_BAR);
	video_setattributetext(0,0xC);
	#ifdef X86
	printf(PANIC_MSG_BAR);
	#endif
	video_resetattributetext();
	klog(LOG_PANIC,"PANIC",reason);
	klog(LOG_PANIC,"PANIC","Debug output:\n");
	klog(LOG_PANIC,"PANIC","Debug register output unimplemented\n");
	halt();
}

void halt()
{
	klog(LOG_INFO,"KERN","Halting...\n");
	#ifdef X86
	asm("hlt");
	#endif
}