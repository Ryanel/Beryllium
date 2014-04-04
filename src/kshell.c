#include <stdio.h>
#include <string.h>
unsigned char command[0xFF];
int command_i = 0;

const char *prompt = "shell@beryllium / $ ";

void kshell_init()
{
	memset(command,0x0,0xFF); //clear commmand
	command_i = 0;
	printf(prompt);
}

void kshell_parse_command(unsigned char *s)
{
	if 		(strcmp(s,"about") == 0)
	{
		printf("Beryllium\n");
	}
	else if (strcmp(s,"sysinfo") == 0)
	{
		printf("System is running\n");
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
		default:
			if(command_i!=0xFF)
			{
				command[command_i++] = input; 
				printf("%c",input);
			}
			break;
	}

}