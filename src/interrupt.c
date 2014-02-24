#include <log.h>
#include <types.h>
#include <interrupt.h>
void io_interrupt_recieve(interrupt_message_t *data)
{
	klog(LOG_DEBUG,"IO","type: %d; danger: %d; queue: %d; from: %d; to: %d; data: 0x%X; handled: %d\n",data->type,data->isDangerous,data->isQueueable,data->from,data->to,data->data,data->isHandled);
	if(data->isHandled)
	{
		return;
	}
	if(data->isDangerous)
	{
		klog(LOG_SEVERE,"IO","Encountered dangerous interrupt!\n");

	}
	if(data->isQueueable)
	{
		//TODO:Add to queue and return
	}

}
/**
Purges the queue and does other things, every clock cycle.
**/
void io_interrupt_docyclic()
{

}

void io_interrupt_handle(interrupt_message_t data)
{

}