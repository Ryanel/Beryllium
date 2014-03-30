#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <driver.h>
#include <log.h>

driver_t * driver_list[0xFF];
int next_driver = 0;
char* driver_decodeclass(int class)
{
	switch(class)
	{
		case 0x01: return "Mass Storage Controller";
		case 0x02: return "Network Controller";
		case 0x03: return "Display Controller";
		case 0x04: return "Multimedia Controller";
		case 0x05: return "Memory Controller";
		case 0x06: return "Bridge Device";
		case 0x07: return "Simple Communication Controller";
		case 0x08: return "Base System Peripheral";
		case 0x09: return "Input Devices";
		case 0x0A: return "Docking Stations";
		case 0x0B: return "Processors";
		case 0x0C: return "Serial Bus Controller";
		case 0x0D: return "Wireless Controller";
		case 0x0E: return "Intelligent I/O Controller";
		case 0x0F: return "Satellite Communication Controller";
		case 0x10: return "Encryption/Decryption Controller";
		case 0x11: return "Data Acquisition and Signal Processing Controller";
		default:    return "Unknown";
	}
}
void driver_printinfo(driver_t *driver)
{
	printf("%s: ",driver->name);
	if(driver->loaded)
	{
		printf("Is loaded\n");
	}
	else
	{
		printf("Is unloaded\n");
	}
	printf("Class: %s ",driver_decodeclass(driver->class));
	printf("\n");
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
			return ret;
		}
		driver->loaded = 1;
		return ret;
	}
	else
	{
		klog(LOG_ERROR,"DRV","Driver %s does not support starting!\n",driver->name);
		return 0xFFFFFFFF;
	}
}