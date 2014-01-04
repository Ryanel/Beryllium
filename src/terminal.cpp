#include <terminal.h>
#include <types.h>
#include <low_textmode.h>

int term_x;
int term_y;
///
///Determines if the screen needs to be scrolled, and scrolls.
///
bool statusBar=true;
const char *statusBarTitle;

void scroll()
{
	if(statusBar)
	{
		if(term_y >= 25)
		{
			textmode_scroll(0,24);

			term_y = 24;
		}
	}
	else
	{
		if(term_y >= 24)
		{
			textmode_scroll(0,23);

			term_y = 23;
		}
	}
}

void terminal_clear_statusbar()
{
	int i = 0;
	while (i!=80)
	{
		textmode_write_color(i,24,' ',STATUS_BAR_ATTRIBUTE);
		i++;
	}
	
}

void terminal_set_statusbar(const char *c)
{
	
	int i = 0;
	terminal_clear_statusbar();
	while (c[i])
	{
		textmode_write_color(i,24,c[i],STATUS_BAR_ATTRIBUTE);
		i++;
	}
	
}

///
///Better than textmode_write, it formats the output at a basic level.
///
void printc(unsigned char c)
{
	if (c == 0x08 && term_x) //Backspace
	{
		term_x--;
	}
	else if (c == 0x09) //Tab
	{
		term_x = (term_x+8) & ~(8-1);
	}
	else if (c == '\r') //Return
	{
	   term_x = 0;
	}

	else if (c == '\n') //Newline
	{
	   term_x = 0;
	   term_y++;
	}
	else if(c >= ' ') //Anything else
	{
		textmode_write(term_x,term_y,c);
		term_x++;
	}

	if (term_x >= 80)
	{
		term_x = 0;
		term_y ++;
	}

	// Scroll the screen if needed.
	scroll();
	// Move the hardware cursor.
	textmode_setcursor(term_x,term_y);
}

///
///Prints a basic string
///
void print(const char *c)
{
	int i = 0;
	while (c[i])
	{
		printc(c[i++]);
	}
}
void terminal_clear()
{
	int i=0;
	while(i!=24)
	{
		textmode_scroll(0,24);
		i++;
	}
	//FIXME: CLEAR THE FRAMEBUFFER INSTEAD
}
///
///Initialises the whole thing
///
void terminal_init()
{
	term_x=0;
	term_y=0;
	textmode_setcursor(0,0);
	terminal_clear();
	terminal_set_statusbar("Terminal");
}
