#include <arm/intergrator-cp/drivers/serial.h>
#include <stdio.h>
#include <system.h>
void arm_integrator_cp_early()
{
	printf("Beryllium %s - version %s (from %s)\n",BERYLLIUM_RELEASE,BERYLLIUM_VERSION,BERYLLIUM_SOURCE);
	for(;;);
}