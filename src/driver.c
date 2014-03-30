#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <driver.h>
#include <log.h>

driver_t * driver_list[0xFF];
int next_driver = 0;

void driver_printinfo(driver_t *driver)
{
	printf("|| %-50s|",driver->name);
	char *status = "";
	if(driver->status == DRIVER_STATUS_ONLINE)
	{
		status = "online";
	}
	else if(driver->status == DRIVER_STATUS_OFFLINE)
	{
		status = "offline";
	}
	else if(driver->status == DRIVER_STATUS_ABORTED)
	{
		status = "aborted";
	}

	printf("%8s|0x%03X|0x%03X ||\n",status,driver->class,driver->type);
}

void driver_list_drivers()
{
	printf("// %-50s|%8s|Class|Type  \\\\\n","Driver Name","Status");
	for(int i = 0; i != 0xFF; i++)
	{
		if(driver_list[i])
		{
			driver_printinfo(driver_list[i]);
		}
	}
}

int init = 1;

void driver_register(driver_t * driver)
{
	if(init)
	{
		for(int i = 0; i != 0xFF; i++)
		{
			driver_list[i] = NULL;
		}
		init = 0;
	}
	driver->driver_id = next_driver;
	driver_list[next_driver] = driver;
	next_driver++;
}

int driver_sendmessage(int class, int type,driver_msg_t * msg)
{
	for(int i = 0; i != 0xFF; i++)
	{
		if(driver_list[i])
		{
			if(driver_list[i]->class == class)
			{
				if(driver_list[i]->type == type)
				{
					return driver_list[i]->recieve(msg);
				}
			}
		}
	}
	return 0xFFFFFFFF;
}

int driver_start(driver_t * driver)
{
	if(driver->start)
	{
		int ret = driver->start();
		if(ret)
		{
			driver->status = DRIVER_STATUS_ABORTED;
			return ret;
		}
		driver->status = DRIVER_STATUS_ONLINE;
		return ret;
	}
	else
	{
		klog(LOG_ERROR,"DRV","Driver '%s' does not support starting!\n",driver->name);
		return 0xFFFFFFFF;
	}
}