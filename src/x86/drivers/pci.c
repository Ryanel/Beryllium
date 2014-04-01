#include <x86/drivers/serial.h>
#include <x86/ports.h>
#include <stdio.h>
#include <driver.h>
driver_t			 pci_driver;

int pci_start()
{
	return 0;
}

void pci_init() {
	pci_driver.start = &pci_start;
	driver_start(&pci_driver);
}