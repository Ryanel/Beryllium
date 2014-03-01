#include <stdio.h>
#include <drivers/timer.h>
#include <drivers/low_textmode.h>
#include <drivers/serial.h>
#include <terminal.h>
#include <interrupt.h>
#include <video.h>
void terminal_clear();
int ct_state = 0;
void panic(const char* reason);
int first_input = 1;
char *command_string = "%%serial/> ";
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

	if((tick % 100) == 0)
	{
		char serial_char = serial_read_nowait();
		if(serial_char != 0)
		{
			if(first_input)
			{
				printf(command_string);
				first_input = 0;
			}
			switch(serial_char)
			{
				case 't':
					printf("Timing internals: %08d hi; %08d low. Device %d\n",timer_getHi(),timer_getLow(),timer_getDevice());
					break;
				case 'i':
					printf("IO interrupt system working: %d\n",isInterruptWorking());
					break;
				case 'v':
					printf("Graphics Support: %d\n",video_graphics_capable());
					break;
				case 'u':
					printf("Uptime: %d sec. (%d minutes)\n",timer_getUptime(),timer_getUptime()/60);
					break;
				case 'q':
					printf("Halting...\n");
					asm("cli");
					printf("This is our final goodbye; there is no way for the kernel to revive itself. You killed it.\n");
					klog(LOG_PANIC,"KERN","Status: Interrupts disabled, kernel halted.\n");
					asm("hlt");
					printf("Kernel was brought back up. Continuing...\n");
					break;
				default:
					printf("%c is not a valid command\n",serial_char);
			}
			printf(command_string);
		}
	}
	return 0;
}