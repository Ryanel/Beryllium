#ifndef INTERRUPT_H
#define INTERRUPT_H

typedef struct {
	int type;
	int isQueueable;
	int data;
	int isHandled;
} interrupt_message_t;

//TYPES
#define IO_TYPE_TIMER 0
#define IO_TYPE_IO 0x1
#define IO_TYPE_EXCEPTION 0x2
//TO
void io_interrupt_recieve(interrupt_message_t *data);
void io_interrupt_docyclic();
void io_interrupt_handle(interrupt_message_t *data);
void io_interrupt_handle_dangerous(interrupt_message_t *data);
#endif