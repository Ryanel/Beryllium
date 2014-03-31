#ifndef DEVICE_H
#define DEVICE_H
#include <driver.h>

#define DEVICE_INTERFACE_KERNEL 0x0
#define DEVICE_INTERFACE_PCI    0x1
#define DEVICE_INTERFACE_USB    0x2
#define DEVICE_INTERFACE_ACPI   0x3

typedef struct {
	uint32_t class;
	uint32_t type;
} kernel_device_config_t;

typedef struct {
	uint32_t type;
	uint32_t flags;
	uint32_t permissions;
	uint32_t interface;
	///Driver
	driver_t driver;
	///PCI Configuration
	//TODO
	///USB Configuration
	//TODO
	///ACPI Configuration
	//TODO
} device_t;
#endif