#include <arm/intergrator-cp/drivers/serial.h>
#include <beryllium/memory.h>
#include <stdio.h>
#include <log.h>
void arm_integrator_cp_early()
{
	serial_print_header();
	klog(LOG_INFO,"LOG","Logging up on arm (via. UART0)\n");
	memory_init();
	for(;;);
}