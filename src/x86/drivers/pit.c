#include <stdio.h>
#include <x86/x86.h>
#include <x86/isr.h>
#include <x86/idt.h>
#include <x86/irq.h>
#include <x86/ports.h>
#include <drivers/serial.h>
#include <drivers/timer.h>
#include <interrupt.h>
unsigned long timer_ticks = 0;
unsigned long timer_ticks_old = 0;

interrupt_message_t *data = 0;

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
	//timer_recieveTick(0);
	io_interrupt_recieve(data);
}

void pit_install()
{
	data->type 			= IO_TYPE_TIMER;
	data->isDangerous	= 0;
	data->isQueueable	= 0;
	data->from 			= IO_FROM_TIMER;
	data->to 			= IO_TO_IO;
	data->isHandled 	= 0;
	data->data			= 0xFA;
	register_interrupt_handler(IRQ0,&timer_handler);
	pit_phase(1000);
}

int pit_has_ticked()
{
	int retval=timer_ticks-timer_ticks_old;
	timer_ticks_old=timer_ticks;
	return retval;
}

void pit_wait(unsigned int ticks)
{
	unsigned int eticks;

	eticks = timer_ticks + ticks;
	while(timer_ticks < eticks)
	{
		
	}
}
