#include <stdio.h>
#include <drivers/serial.h>
#include <drivers/timer.h>

void terminal_clear();
void panic(const char* reason);
int cyclic_tasks(int tick)
{
	if((tick % 10) == 0)
	{
		if(serial_received())
		{
			char c = serial_read_nowait();
			
			if(c == 'p')
			{
				panic("User caused kernel to crash over interactive console\n");
			}
		}
	}
	return 0;
}