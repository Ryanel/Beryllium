#include <log.h>
#include <types.h>
#include <interrupt.h>
#include <drivers/timer.h>
void io_interrupt_recieve(interrupt_message_t *data)
{
	if(data->isHandled)
	{
		return;
	}
	if(data->isDangerous)
	{
		klog(LOG_SEVERE,"IO","Encountered dangerous interrupt!\n");
		io_interrupt_handle_dangerous(data);
		return;

	}
	if(data->isQueueable)
	{
		//TODO:Add to queue and return
	}
	io_interrupt_handle(data);
}
/**
Purges the queue and does other things, every clock cycle.
**/
void io_interrupt_docyclic()
{
	return;
}

void io_interrupt_handle(interrupt_message_t *data)
{
	if(data->isDangerous)
	{
		return;
	}
	
	switch(data->type)
	{
		case IO_TYPE_TIMER:
			timer_recieveTick(data->data);
			break;
		default:
			klog(LOG_DEBUG,"IO:Unknown","type: %d; danger: %d; queue: %d; from: %d; to: %d; data: 0x%X; handled: %d\n",data->type,data->isDangerous,data->isQueueable,data->from,data->to,data->data,data->isHandled);
	}
	data->isHandled = 1;

}

void io_interrupt_handle_dangerous(interrupt_message_t *data)
{
	if(!data->isDangerous)
	{
		return;
	}
}