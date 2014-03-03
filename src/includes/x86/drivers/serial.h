#ifndef DRVSERIAL_H
#define DRVSERIAL_H
#define COM1 0x3f8
void serial_init();
int serial_received();
char serial_read();
char serial_read_nowait() ;
int serial_is_transmit_empty();
void serial_write(char a);
void serial_print(const char *c);
#endif
