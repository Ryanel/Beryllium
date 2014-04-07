#include <stdio.h>
#include <string.h>
#include <system.h>
#include <error.h>
#include <log.h>
#include <beryllium/vfs.h>
#include <beryllium/watchdog.h>
#include <beryllium/timing.h>
#include <beryllium/device.h>
char command[0xFF];
int command_i = 0;
const char *prompt = "shell@beryllium / # ";
void kshell_init()
{
	memset(command,0x0,0xFF); //clear commmand
	command_i = 0;
	printf(prompt);
}

extern tree_t   * device_tree;

void kshell_parse_command(char *s)
{
	if 		(strcmp(s,"about") == 0)
	{
		printf("Beryllium %s (v.%s)-%s(%s)\n",BERYLLIUM_RELEASE,BERYLLIUM_VERSION,BERYLLIUM_SOURCE,"000000");
	}
	else if (strcmp(s,"sysinfo") == 0)
	{
		printf("Beryllium - in kshell. No user\n");
	}
	else if (strcmp(s,"crash") == 0)
	{
		halt();
	}
	else if (strcmp(s,"ls") == 0)
	{
		vfs_print_tree_node(vfs_tree->root, 0);
	}
	else if (strcmp(s,"lsmod") == 0)
	{
		device_tree_enumerate(device_tree->root, 0);
	}
	else if (strcmp(s,"help") == 0)
	{
		printf("help\t| Does not help you\n");
		printf("crash\t| Causes the kernel to crash\n");
		printf("about\t| Shows information about the kernel\n");
		printf("ls\t| Shows the directory tree\n");
		printf("lsmod\t| Shows the kernel mod tree\n");
		printf("sysinfo\t| Debug kernel information\n");
		printf("wd\t| Watchdog information\n");
		printf("wdcrash\t| Crash the watchdog\n");
		printf("timers\t| Shows all current timers\n");
	}
	else if (strcmp(s,"wd") == 0)
	{
		printf("Watchdog: %d kernel hangups, %d total hangups\n",wd_get_kmain_hangups(),wd_get_hangups());
	}
	else if (strcmp(s,"wdcrash") == 0)
	{
		wd_disable();
	}
	else if (strcmp(s,"timers") == 0)
	{
		list_timers();
	}
	else if (strcmp(s,"") == 0)
	{
	}
	else
	{
		printf("kshell: Command '%s' not found. Try help\n",s);
	}
	printf(prompt);
}
void kshell_parse_char(unsigned char input)
{
	switch(input)
	{
		case 0xFF:
			return;
		case '\n':
			printf("\n");
			kshell_parse_command(command);
			memset(&command,0,0xFF);
			command_i = 0;
			return;
		case 0x0:
			return;
		case '\b':
			if(command[0]==0)
            {
                return; //Dont erase from the buffer if we dont have anything!
            }
            printf("\b \b");
			command[command_i--] = 0;
			return;
		default:
			if(command_i!=0xFF)
			{
				command[command_i++] = input; 
				printf("%c",input);
			}
			break;
	}

}
