#include <log.h>
#include <types.h>
#include <interrupt.h>
#include <timer.h>

int interrupt_sys_working = 0;

int isInterruptWorking()
{
	return interrupt_sys_working;
}

void io_interrupt_recieve(interrupt_message_t *data)
{
	interrupt_sys_working = 1;
	if(data->isHandled)
	{
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
	
	switch(data->type)
	{
		case IO_TYPE_TIMER:
			timer_recieveTick(data->data);
			io_interrupt_docyclic();
			break;
		case IO_TYPE_IO:
			break;
		case IO_TYPE_EXCEPTION:
			klog(LOG_SEVERE,"IO","Exception type 0x%X!\n",data->data);
			asm("hlt");
			break;
		default:
			klog(LOG_DEBUG,"IO:Unknown","type: %d; queue: %d; data: 0x%X; handled: %d\n",data->type,data->isQueueable,data->data,data->isHandled);
	}
	data->isHandled = 1;

}
