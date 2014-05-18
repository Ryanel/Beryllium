#include <beryllium/video.h>
#include <beryllium/watchdog.h>
#include <beryllium/timing.h>
#include <beryllium/timer.h>
#include <stdio.h>
#include <beryllium/debug.h>
#include <stdio.h>
#include <x86/cpuid.h>
#include <terminal.h>
#ifdef DEBUG
#ifdef X86
char kb_read();
void terminal_clear();

int is_debugging = 0;
int is_selecting = 0;
uint64_t debug_called = 0;
unsigned int selecting_index = 0;
unsigned int selecting_max = 0;
unsigned char last_command = ' ';
int redraw_screen = 1;
int time_to_update = 0;
char current_screen = 0;
char *current_screen_name;
void dbg_screen_0()
{
	printf_at_cc(0,1,0x0F,"The Beryllium Debug Shell");
	printf_at_cc(0,2,0x0F,"This shell contains debug information about the current status of Beryllium");
	printf_at_cc(0,3,0x0F,"Below are a list of commands");
	printf_at_cc(0,4,0x0F," - # | Switch Screens.");
	printf_at_cc(0,5,0x0F," -TAB| Select");
	printf_at_cc(0,6,0x0F," - q | Quit Debug Mode");
	redraw_screen  = 0;
}

int screen_1_pos_feature = 0;

void dbg_screen_1_printfeature(int row, int extended,char * name,int feature)
{
	printf_at_cc(row * 20,screen_1_pos_feature + 2,0x8F,"%-10s        %d|",name,cpu_feature_avalable(feature));
	screen_1_pos_feature++;
}

void dbg_screen_1()
{
	printf_at_cc(0,1,0xF0,"CPU FEATURES                                                ");
    dbg_screen_1_printfeature(0,0,"FPU  ",CPUID_FEAT_EDX_FPU  );        
    dbg_screen_1_printfeature(0,0,"VME  ",CPUID_FEAT_EDX_VME  );        
    dbg_screen_1_printfeature(0,0,"DE   ",CPUID_FEAT_EDX_DE   );        
    dbg_screen_1_printfeature(0,0,"PSE  ",CPUID_FEAT_EDX_PSE  );        
    dbg_screen_1_printfeature(0,0,"TSC  ",CPUID_FEAT_EDX_TSC  );        
    dbg_screen_1_printfeature(0,0,"MSR  ",CPUID_FEAT_EDX_MSR  );        
    dbg_screen_1_printfeature(0,0,"PAE  ",CPUID_FEAT_EDX_PAE  );        
    dbg_screen_1_printfeature(0,0,"MCE  ",CPUID_FEAT_EDX_MCE  );        
    dbg_screen_1_printfeature(0,0,"CX8  ",CPUID_FEAT_EDX_CX8  );        
    dbg_screen_1_printfeature(0,0,"APIC ",CPUID_FEAT_EDX_APIC );        
    dbg_screen_1_printfeature(0,0,"SEP  ",CPUID_FEAT_EDX_SEP  );        
    dbg_screen_1_printfeature(0,0,"MTRR ",CPUID_FEAT_EDX_MTRR );        
    dbg_screen_1_printfeature(0,0,"PGE  ",CPUID_FEAT_EDX_PGE  );        
    dbg_screen_1_printfeature(0,0,"MCA  ",CPUID_FEAT_EDX_MCA  );        
    dbg_screen_1_printfeature(0,0,"CMOV ",CPUID_FEAT_EDX_CMOV );        
    dbg_screen_1_printfeature(0,0,"PAT  ",CPUID_FEAT_EDX_PAT  );        
    dbg_screen_1_printfeature(0,0,"PSE36",CPUID_FEAT_EDX_PSE36);        
    dbg_screen_1_printfeature(0,0,"PSN  ",CPUID_FEAT_EDX_PSN  );        
    dbg_screen_1_printfeature(0,0,"CLF  ",CPUID_FEAT_EDX_CLF  );        
    dbg_screen_1_printfeature(0,0,"DTES ",CPUID_FEAT_EDX_DTES );        
    dbg_screen_1_printfeature(0,0,"ACPI ",CPUID_FEAT_EDX_ACPI );
    dbg_screen_1_printfeature(0,0,"MMX  ",CPUID_FEAT_EDX_MMX  );     
	screen_1_pos_feature = 0;   
    dbg_screen_1_printfeature(1,0,"FXSR ",CPUID_FEAT_EDX_FXSR );        
    dbg_screen_1_printfeature(1,0,"SSE  ",CPUID_FEAT_EDX_SSE  );        
    dbg_screen_1_printfeature(1,0,"SSE2 ",CPUID_FEAT_EDX_SSE2 );        
    dbg_screen_1_printfeature(1,0,"SS   ",CPUID_FEAT_EDX_SS   );        
    dbg_screen_1_printfeature(1,0,"HTT  ",CPUID_FEAT_EDX_HTT  );        
    dbg_screen_1_printfeature(1,0,"TM1  ",CPUID_FEAT_EDX_TM1  );        
    dbg_screen_1_printfeature(1,0,"IA64 ",CPUID_FEAT_EDX_IA64 );        
    dbg_screen_1_printfeature(1,0,"PBE  ",CPUID_FEAT_EDX_PBE  );
	printf_at_cc(1 * 20 ,screen_1_pos_feature + 2 ,0xF0,"EXTENDED FEATURES  |"); screen_1_pos_feature++;
    dbg_screen_1_printfeature(1,1,"SSE3   ",CPUID_FEAT_ECX_SSE3   );
    dbg_screen_1_printfeature(1,1,"PCLMUL ",CPUID_FEAT_ECX_PCLMUL );
    dbg_screen_1_printfeature(1,1,"DTES64 ",CPUID_FEAT_ECX_DTES64 );
    dbg_screen_1_printfeature(1,1,"MONITOR",CPUID_FEAT_ECX_MONITOR);
    dbg_screen_1_printfeature(1,1,"DS_CPL ",CPUID_FEAT_ECX_DS_CPL );
    dbg_screen_1_printfeature(1,1,"VMX    ",CPUID_FEAT_ECX_VMX    );
    dbg_screen_1_printfeature(1,1,"SMX    ",CPUID_FEAT_ECX_SMX    );
    dbg_screen_1_printfeature(1,1,"EST    ",CPUID_FEAT_ECX_EST    );
    dbg_screen_1_printfeature(1,1,"TM2    ",CPUID_FEAT_ECX_TM2    );
    dbg_screen_1_printfeature(1,1,"SSSE3  ",CPUID_FEAT_ECX_SSSE3  );
    dbg_screen_1_printfeature(1,1,"CID    ",CPUID_FEAT_ECX_CID    );
    dbg_screen_1_printfeature(1,1,"FMA    ",CPUID_FEAT_ECX_FMA    );
    dbg_screen_1_printfeature(1,1,"CX16   ",CPUID_FEAT_ECX_CX16   );
	screen_1_pos_feature = 0;  
    dbg_screen_1_printfeature(2,1,"ETPRD  ",CPUID_FEAT_ECX_ETPRD  );
    dbg_screen_1_printfeature(2,1,"PDCM   ",CPUID_FEAT_ECX_PDCM   );
    dbg_screen_1_printfeature(2,1,"DCA    ",CPUID_FEAT_ECX_DCA    );
    dbg_screen_1_printfeature(2,1,"SSE4_1 ",CPUID_FEAT_ECX_SSE4_1 );
    dbg_screen_1_printfeature(2,1,"SSE4_2 ",CPUID_FEAT_ECX_SSE4_2 );
    dbg_screen_1_printfeature(2,1,"x2APIC ",CPUID_FEAT_ECX_x2APIC );
    dbg_screen_1_printfeature(2,1,"MOVBE  ",CPUID_FEAT_ECX_MOVBE  );
    dbg_screen_1_printfeature(2,1,"POPCNT ",CPUID_FEAT_ECX_POPCNT );
    dbg_screen_1_printfeature(2,1,"AES    ",CPUID_FEAT_ECX_AES    );
    dbg_screen_1_printfeature(2,1,"XSAVE  ",CPUID_FEAT_ECX_XSAVE  );
    dbg_screen_1_printfeature(2,1,"OSXSAVE",CPUID_FEAT_ECX_OSXSAVE);
    dbg_screen_1_printfeature(2,1,"AVX    ",CPUID_FEAT_ECX_AVX    );
	screen_1_pos_feature = 0;  
	redraw_screen  = 0;
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
	unsigned int index = 0;
	unsigned int current_time = timer_getHi();
	char *unit = "ms";
	unsigned int print_index = 0;
	uint32_t sel = 0;
	for(index = 0;index<0xFF;index++)
	{
		if(handlers[index].fire_tick == 0)
		{
			continue;
		}
		else
		{
			sel++;
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
					printf_at_cc(0,3 + print_index++,0xF0,"[%02x]:%-54s|%8d%2s|%7dms",index,handlers[index].name,rep_in,unit,handlers[index].repeat_rate);
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
	redraw_screen  = 1;
}

void dbg_update_display()
{
	debug_called++;
	if(!is_debugging)
	{
		return;
	}
	if(!redraw_screen)
	{
		for(int i = 0; i!=80; i++)
		{
			video_printcoloredchar(i,0,' ',0xE0);
		}
		printf_at_cc(0,0,0xE0,"Debug Mode | Screen %d %-10s | Command: %c |",current_screen,current_screen_name,last_command);
	}
	if(redraw_screen)
	{
		terminal_clear();
		video_printstring(74,24,"DEBUG"); //Title
		for(int i = 0; i!=80; i++)
		{
			video_printcoloredchar(i,0,' ',0xE0);
		}
		printf_at_cc(0,0,0xE0,"Debug Mode | Screen %d %-10s | Command: %c | !",current_screen,current_screen_name,last_command);
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
			redraw_screen  = 1;
			break;
		case '1':
			current_screen = 1;
			current_screen_name = "CPU Info";
			redraw_screen  = 1;
			break;
		case '2':
			current_screen = 2;
			current_screen_name = "Timers";
			redraw_screen  = 1;
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
		case 'q':
			is_debugging = 0;
			timing_deregister_timer(dbg_update_display);
		default:
			break;
	}
	
}

void dbg_loop()
{
	while(is_debugging)
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
#else
void dbg_start()
{
	printf("Debug mode not compiled in! -- Returning\n");
}
#endif
#endif
