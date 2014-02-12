#include <stdio.h>
#include <drivers/timer.h>

void terminal_clear();
void panic(const char* reason);
int cyclic_tasks(int tick)
{
	if((tick % 10) == 0)
	{
	
	}
	return 0;
}