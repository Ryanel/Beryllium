#ifndef INTERRUPT_H
#define INTERRUPT_H

typedef struct {
	int type;
	int isDangerous;
	int isQueueable;
	int from;
	int to;
	int data;
	int isHandled;
} interrupt_message_t;

//TYPES
#define IO_TYPE_TIMER 0
#define IO_TYPE_IO 1

//TO
#define IO_TO_TIMER 0
#define IO_TO_IO 1
//FROM
#define IO_FROM_TIMER IO_TO_TIMER
#define IO_FROM_IO IO_TO_IO

void io_interrupt_recieve(interrupt_message_t *data);
void io_interrupt_docyclic();
void io_interrupt_handle(interrupt_message_t *data);
void io_interrupt_handle_dangerous(interrupt_message_t *data);
#endif