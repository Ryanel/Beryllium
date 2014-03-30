#include <x86/low/isr.h>
#include <x86/low/idt.h>
#include <x86/low/irq.h>
#include <x86/ports.h>
#include <timer.h>
#include <driver.h>
#include <string.h>
#include <stdio.h>

#define KB_TRAP_CTL    0x01
#define KB_TRAP_LSHIFT 0x02
#define KB_TRAP_RSHIFT 0x03
#define KB_TRAP_ALT    0x04
#define KB_CAPS        0x05
#define KB_F1          0x06

driver_t			 keyboard_driver;

char buffer[256];

//Taken shamelessly from Bran's kernel development tutorial
unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    KB_TRAP_CTL,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   KB_TRAP_LSHIFT,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   KB_TRAP_RSHIFT,				/* Right shift */
  '*',
    KB_TRAP_ALT,	/* Alt */
  ' ',	/* Space bar */
    KB_CAPS,	/* Caps lock */
    KB_F1,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};		
unsigned char kbdus_caps[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    KB_TRAP_CTL,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   KB_TRAP_LSHIFT,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   KB_TRAP_RSHIFT,				/* Right shift */
  '*',
    KB_TRAP_ALT,	/* Alt */
  ' ',	/* Space bar */
    KB_CAPS,	/* Caps lock */
    KB_F1,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};		

int kb_shift = 0;
int kb_alt   = 0;
int kb_ctl   = 0;

void kb_handler(struct regs *r)
{
	unsigned char scancode;
    scancode = inb(0x60);

    if (scancode & 0x80)
    {
        if(scancode == 0xAA) //LSHIFT
        {
        	kb_shift = 0;
        }
    }
    else
    {
    	if(kbdus[scancode] == KB_TRAP_CTL)
    	{
    		kb_ctl = 1;
    	}
    	else if(kbdus[scancode] == KB_TRAP_LSHIFT)
    	{
    		kb_shift = 1;
    	}
    	else
    	{

    	}
    }
}

int kb_start()
{
	register_interrupt_handler(IRQ1,&kb_handler);
	return 0;
}

int kb_stop()
{
	return 0xFFFFFFFF;
}

int kb_recieve(driver_msg_t * data)
{
	return 0;
}
void kb_init()
{
	keyboard_driver.class = 0x9;
	keyboard_driver.type = 0x00;
	strcpy(keyboard_driver.name,"Keyboard Controller");
	keyboard_driver.start = &kb_start;
	keyboard_driver.stop = &kb_stop;
	keyboard_driver.recieve = &kb_recieve;
	driver_register( &keyboard_driver );
	driver_start(&keyboard_driver);
}