#include <drivers/serial.h>
#include <ports.h>
#include <stdio.h>
void serial_init() {
	outb(COM1 + 1, 0x00);    // Disable all interrupts
	outb(COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	outb(COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	outb(COM1 + 1, 0x00);    //                  (hi byte)
	outb(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
	outb(COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	outb(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}
int serial_received() {
	return inb(COM1 + 5) & 1;
}

char serial_read() {
	while (serial_received() == 0);

	return inb(COM1);
}

char serial_read_nowait() {
	if(serial_received() == 1)
	{
		return inb(COM1);
	}
	return 0;
	
}
int serial_is_transmit_empty() {
	return inb(COM1 + 5) & 0x20;
}

void serial_write(char a) {
	while (serial_is_transmit_empty() == 0);
	outb(COM1,a);
}

void serial_print(const char *c)
{
	int i = 0;
	while (c[i])
	{
		serial_write(c[i++]);
	}
}