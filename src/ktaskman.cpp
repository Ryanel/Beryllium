#include <stdio.h>
#include <drivers/serial.h>
int cyclic_tasks(int tick)
{
	if((tick % 1000) == 0)
	{
		if(serial_received())
		{
			printf("Recieved key (serial):%c\n",serial_read_nowait());
		}
	}
	return 0;
}