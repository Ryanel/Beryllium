#include <system.h>
#include <stdio.h>
void serial_print_header(char *board,char *cpu_revision)
{
	for(int i = 0; i < 8; i++)
		serial_printf("----------");
	serial_printf("\n");
	serial_printf("Beryllium %s - version %s (from %s)\n",BERYLLIUM_RELEASE,BERYLLIUM_VERSION,BERYLLIUM_SOURCE);
	serial_printf("Output Device: Serial\n");
	serial_printf("Architecture: %s (%s)\n",ARCH_STRING,board);
	serial_printf("Processor: %s\n",cpu_revision);
	for(int i = 0; i < 8; i++)
		serial_printf("----------");
	serial_printf("\n");
}