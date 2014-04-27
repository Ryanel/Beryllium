#include <beryllium/video.h>
#include <beryllium/watchdog.h>
#include <beryllium/timing.h>
#include <beryllium/timer.h>
#include <stdio.h>
int is_debugging = 0;
int is_selecting = 0;
unsigned int selecting_index = 0;
unsigned int selecting_max = 0;
unsigned char last_command = ' ';

int time_to_update = 0;
char current_screen = 0;
char *current_screen_name;
void dbg_screen_0()
{
	printf_at_cc(0,1,0x0F,"The Beryllium Debug Shell");
	printf_at_cc(0,2,0x0F,"This shell contains debug information about the current status of Beryllium");
	printf_at_cc(0,3,0x0F,"Below are a list of commands");
	printf_at_cc(0,4,0x0F," - # | Switch Screens.");
	printf_at_cc(0,5,0x0F," - q | Quit Debug Mode");
}

void dbg_screen_1()
{
}
void list_timers_dbg();
extern timer_t handlers[0xFF];
void dbg_screen_2()
{
	int total = 0;
	for(int index = 0;index<0xFF;index++)
	{
		if(handlers[index].fire_tick == 0)
		{
			continue;
		}
		else
		{
			total++;
		}
	}
	printf_at(0,1,"Timers: %d",total);
	selecting_max = total - 1;
	printf_at_cc(0,2,0xF0," ID |%-50sNAME|NEXT    ms|REP    ms","",0,0);
	int index = 0;
	int current_time = timer_getHi();
	char *unit = "ms";
	int print_index = 0;
	for(index = 0;index<0xFF;index++)
	{
		if(handlers[index].fire_tick == 0)
		{
			continue;
		}
		else
		{
			int rep_in = handlers[index].fire_tick - current_time;
			if(rep_in > 1000)
			{
				unit = "s";
				rep_in = rep_in / 1000;
			}
			else
			{
				unit = "ms";
			}
			if(is_selecting)
			{
				if(index == selecting_index)
				{
					printf_at_cc(0,3 + print_index++,0xB0,"[%02x]:%-54s|%8d%2s|%7dms",index,handlers[index].name,rep_in,unit,handlers[index].repeat_rate);
					continue;
				}
			}
			printf_at(0,3 + print_index++,"[%02x]:%-54s|%8d%2s|%7dms",index,handlers[index].name,rep_in,unit,handlers[index].repeat_rate);
		}
	}
	
	//Handle commands
	if(is_selecting)
	{
		switch(last_command)
		{
			case 'x':
				handlers[selecting_index].repeat_rate = 0;
				break;
			case '+':
				handlers[selecting_index].repeat_rate++;
				break;
			case '-':
				handlers[selecting_index].repeat_rate--;
				break;
			default:
				break;
		}
	}
}

void dbg_update_display()
{
	if(!is_debugging)
	{
		return;
	}
	terminal_clear();
	video_printstring(74,24,"DEBUG"); //Title
	for(int i = 0; i!=80; i++)
	{
		video_printcoloredchar(i,0,' ',0xEF);
	}
	printf_at_cc(0,0,0xE0,"Debug Mode | Screen %d %-10s | Command: %c |",current_screen,current_screen_name,last_command);
	switch(current_screen)
	{
		case 0:
			dbg_screen_0();
			break;
		case 1:
			dbg_screen_1();
			break;
		case 2:
			dbg_screen_2();
			break;
	}
}

void dbg_parse_char(unsigned char c)
{
	if(c)
	{
		last_command = c;
	}
	switch(c)
	{
		case '0':
			current_screen = 0;
			current_screen_name = "Help";
			break;
		case '1':
			current_screen = 1;
			current_screen_name = "Main";
			break;
		case '2':
			current_screen = 2;
			current_screen_name = "Timers";
			break;
		case 'j':
			if(selecting_index != selecting_max)
			{
				selecting_index++;
			}
			break;
		case 'k':
			if(selecting_index != 0)
			{
				selecting_index--;
			}
			break;
		case '\t':
			if(!is_selecting)
			{
				is_selecting = 1;
				selecting_index = 0;
			}
			else
			{
				is_selecting = 0;
				selecting_index = 0;
			}
			break;
		default:
			break;
	}
	
}

void dbg_loop()
{
	while(1)
	{
		//dbg_update_display();
		dbg_parse_char(kb_read());
		wd_notify(WD_NOTIFY_KMAIN);
	}
}

void dbg_start()
{
	is_debugging = 1;
	current_screen_name = "Help";
	timing_register_timer("dbg_screen",10,dbg_update_display, 16);
	dbg_loop();
}
