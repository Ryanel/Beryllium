#include <log.h>
void initialise_syscalls();
int init_syscall()
{
	klog(LOG_DEBUG,"SYS","Enabling System Calls...\n");
	initialise_syscalls();
	return 0;
}