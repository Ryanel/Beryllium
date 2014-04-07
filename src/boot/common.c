#include <system.h>
#include <stdio.h>
void serial_print_header()
{
	serial_printf("-------------------------------------------------------------\n");
	serial_printf("Beryllium %s - version %s (from %s)\n",BERYLLIUM_RELEASE,BERYLLIUM_VERSION,BERYLLIUM_SOURCE);
	serial_printf("Output Device: Serial\n");
	serial_printf("Architecture: %s\n",ARCH_STRING);
	serial_printf("-------------------------------------------------------------\n");
}