#include <stdio.h>
#include <drivers/timer.h>
#include <drivers/low_textmode.h>
#include <terminal.h>
void terminal_clear();
int ct_state = 0;
void panic(const char* reason);
int cyclic_tasks(int tick)
{
	if((tick % 50) == 0)
	{
		if(ct_state == 0)
		{
			textmode_write_color(79,24,'|',STATUS_BAR_ATTRIBUTE);
			ct_state = 1;
		}
		else if (ct_state == 1)
		{
			textmode_write_color(79,24,'/',STATUS_BAR_ATTRIBUTE);
			ct_state = 2;
		}
		else if (ct_state == 2)
		{
			textmode_write_color(79,24,'-',STATUS_BAR_ATTRIBUTE);
			ct_state = 3;
		}
		else if (ct_state == 3)
		{
			textmode_write_color(79,24,'\\',STATUS_BAR_ATTRIBUTE);
			ct_state = 0;
		}
	}
	if((tick % 10) == 0)
	{

	}
	return 0;
}