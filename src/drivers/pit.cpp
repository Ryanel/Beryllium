#include <log.h>
#include <x86.h>
#include <isr.h>
#include <idt.h>
#include <irq.h>
#include <ports.h>
#include <drivers/serial.h>
#include <drivers/timer.h>
void pit_phase(int hz)
{
	int divisor = 1193180 / hz;       /* Calculate our divisor */
	outb(0x43, 0x36);             /* Set our command byte 0x36 */
	outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
	outb(0x40, divisor >> 8);     /* Set high byte of divisor */
}
int timer_ticks = 0;
int timer_ticks_old = 0;


void timer_handler(struct regs *r)
{
	timer_ticks++;
	timer_recieveTick(0);

}

void pit_install()
{
	register_interrupt_handler(IRQ0,&timer_handler);
	pit_phase(1000);
}

int pit_has_ticked()
{
	int retval=timer_ticks-timer_ticks_old;
	timer_ticks_old=timer_ticks;
	return retval;
}

void pit_wait(int ticks)
{
	unsigned int eticks;

	eticks = timer_ticks + ticks;
	while(timer_ticks < eticks)
	{
		
	}
}
