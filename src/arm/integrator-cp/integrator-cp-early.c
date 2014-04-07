#include <arm/intergrator-cp/drivers/serial.h>
#include <beryllium/memory.h>
#include <stdio.h>
#include <log.h>
#include <types.h>
#include <beryllium/video.h>
void arm_integrator_cp_early()
{
	serial_print_header();
	klog(LOG_INFO,"LOG","Logging up on ARM (via. UART0)\n");
	uint32_t pc;
	asm("ldrh   %0, [pc, #6]" :"=r"(pc) : : "memory");
	klog(LOG_INFO,"ARM","Program counter at ~0x%X\n");
	memory_init();
	klog(LOG_DEBUG,"ARM","Clearing screen!\n");
	qemu_pl110_start();
	video_printchar(1,1, 'B');
	for(;;);
}