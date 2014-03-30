#include <x86/low/isr.h>
#include <x86/low/idt.h>
#include <x86/low/irq.h>
#include <x86/ports.h>
#include <timer.h>
#include <driver.h>
#include <string.h>
#include <stdio.h>

unsigned long        timer_ticks     = 0;
unsigned long        timer_ticks_old = 0;

driver_t			 pit_driver;

void pit_phase(int hz)
{
	int divisor = 1193180 / hz;       /* Calculate our divisor */
	outb(0x43, 0x36);             /* Set our command byte 0x36 */
	outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
	outb(0x40, divisor >> 8);     /* Set high byte of divisor */
}

void timer_handler(struct regs *r)
{
	if(r->int_no) {} // Used to disable unused argument warning
	timer_ticks++;
	timer_recieveTick(0);
}

int pit_start()
{
	register_interrupt_handler(IRQ0,&timer_handler);
	pit_phase(1000);
	return 0;
}

int pit_stop()
{
	return 0xFFFFFFFF;
}

int pit_recieve(driver_msg_t * data)
{
	printf("pit: recieved message...\n");
	if(data->type == 0) //Change clockrate
	{
		int rate = (int)data->data;
		pit_phase(rate);
		printf("pit: changed clockerate to %dhz\n",rate);
	}
}
void pit_init()
{
	pit_driver.class = 0x8;
	pit_driver.type = 0x01;
	strcpy(pit_driver.name,"Programmable Interrupt Timer");
	pit_driver.start = &pit_start;
	pit_driver.stop = &pit_stop;
	pit_driver.recieve = &pit_recieve;
	driver_register( &pit_driver );
	driver_start(&pit_driver);
}