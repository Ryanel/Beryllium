#include <arm/intergrator-cp/drivers/serial.h>
#include <arm/intergrator-cp/arm4-xrq.h>
#include <beryllium/memory.h>
#include <stdio.h>
#include <log.h>
#include <types.h>
#include <boot/kmain.h>
#include <beryllium/video.h>
#include <beryllium/timer.h>
void icp_timer_setup();
void arm_integrator_cp_early()
{
	serial_print_header("Integrator-CP","ARM4");
	klog(LOG_INFO,"LOG","Logging up on ARM (via. UART0)\n");
	uint32_t pc;
	asm("ldrh   %0, [pc, #6]" :"=r"(pc) : : "memory");
	klog(LOG_INFO,"ARM","Program counter at ~0x%X\n");
	arm4_xrqsetup();
	icp_timer_setup();
	uint32_t i = 0;
	klog(LOG_INFO,"ARM","Waiting for timer to fire... ");
	while(timer_getHi() == 0)
	{
		i++;
		if(i > 0xFFFFFF)
		{
			printf("it didn't.\n");
			klog(LOG_INFO,"ARM","Halting!\n");
			return;
		}
	}
	printf("Fired\n");
	memory_init();
	kmain();
	for(;;);
}