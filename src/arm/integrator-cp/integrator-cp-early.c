#include <arm/intergrator-cp/drivers/serial.h>
#include <beryllium/memory.h>
#include <stdio.h>
#include <log.h>
#include <types.h>
void arm_integrator_cp_early()
{
	serial_print_header();
	klog(LOG_INFO,"LOG","Logging up on arm (via. UART0)\n");
	uint32_t pc;
	asm("ldrh   %0, [pc, #6]" :"=r"(pc) : : "memory");
	klog(LOG_INFO,"ARM","Program counter at ~0x%X\n");
	memory_init();
	for(;;);
}