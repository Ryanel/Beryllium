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
	pci_driver.class = 0x0;
	pci_driver.type = 0x0;
	strcpy(pci_driver.name,"PCI Enumerator");
	pci_driver.start = &pci_start;
	driver_register( &pci_driver );
	driver_start(&pci_driver);
}