//pit.cpp
#include <log.h>
#include <x86.h>
//#include <irq.h>
#include <isr.h>
#include <idt.h>
void irq_install_handler(int no);
#include <ports.h>
#include <drivers/serial.h>
void pit_phase(int hz)
{
	int divisor = 1193180 / hz;       /* Calculate our divisor */
	outb(0x43, 0x36);             /* Set our command byte 0x36 */
	outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
	outb(0x40, divisor >> 8);     /* Set high byte of divisor */
}
int timer_ticks = 0;
int timer_ticks_old = 0;

int getUptime();
void timer_handler(struct regs *r)
{
	/* Increment our 'tick count' */
	timer_ticks++;
	//printf("One second has passed\n");
	if((timer_ticks % 1000)==0)
	{
		klog(LOG_INFO,"PIT","Timer is working! Uptime:%d seconds\n",getUptime());
	}
}

void pit_install()
{
	irq_install_handler(0);
	pit_phase(1000);
}

int pit_has_ticked()
{
	int retval=timer_ticks-timer_ticks_old;
	timer_ticks_old=timer_ticks;
	return retval;
}

int getUptime()
{
	return timer_ticks/1000;
}

void pit_wait(int ticks)
{
	unsigned int eticks;

	eticks = timer_ticks + ticks;
	while(timer_ticks < eticks)
	{
		
	}
}