#include <beryllium/video.h>
#include <beryllium/watchdog.h>
int is_debugging = 0;

unsigned char last_command = 0;

void dbg_update_display()
{
	if(!is_debugging)
	{
		return;
	}
	video_printchar(79,24, 'D');
	video_printstring(0,0,"Beryllium Debug Mode");
}

void dbg_parse_char(unsigned char c)
{
	last_command = c;
}

void dbg_loop()
{
	while(1)
	{
		dbg_update_display();
		dbg_parse_char(kb_read());
		wd_notify(WD_NOTIFY_KMAIN);
	}
}

void dbg_start()
{
	is_debugging = 1;
	dbg_loop();
}
