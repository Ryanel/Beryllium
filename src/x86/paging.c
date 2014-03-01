#include <types.h>
#include <x86/low_paging.h>
#include <log.h>
#include <x86/x86.h>
#include <x86/isr.h>
#include <x86/idt.h>
#include <x86/irq.h>
#include <x86/placement_malloc.h>
#include <string.h>

void paging_init()
{
	klog(LOG_SEVERE,"ERROR","Paging is not implemented\n");
}
