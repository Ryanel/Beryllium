#include <boot/kmain.h>
#include <boot/multiboot.h>

#include <terminal.h>
#include <stdio.h>
#include <log.h>

#include <x86/x86.h>
#include <x86/drivers/serial.h>
#include <elf.h>

uint32_t initrd_location;
uint32_t initrd_end;
extern uint32_t placement_address;
extern elf_t kernel_elf;
/**
 Get the kernel up and running as fast as possible
**/
void kernel_x86_binding_init(volatile int magic,volatile struct multiboot *mboot)
{
	#ifdef IS_RELEASE
	klog_setmask(LOG_INFO);
	#endif
	terminal_init();

	#ifdef ENABLE_SERIAL
	serial_init();
	#endif
	klog(LOG_INFO,"x86","Loading x86 components...\n");
	if(magic!=0x2BADB002)
	{
		printf("Multiboot Magic number : 0x%X! It should equal 0x2BADB002. Halting...\n",magic);
		return;
	}
	//Set kernel properties from multiboot info.
	klog(LOG_INFO,"MBT","%d modules loaded with kernel\n",mboot->mods_count);
	if(mboot->mods_count != 0)
	{
		initrd_location = *((uint32_t*)mboot->mods_addr);
		initrd_end = *(uint32_t*)(mboot->mods_addr+4);
		placement_address = initrd_end;
	}
	#ifdef KERNEL_SYMBOLS
	if(mboot->num > 0)
	{
		klog(LOG_INFO,"ELF","Loading %d kernel symbols from symbol table...\n",mboot->num);
		kernel_elf = elf_from_multiboot(mboot);
	}
	#endif
	init_x86();
	klog(LOG_DEBUG,"MEM","Loading memory map...\n");
	multiboot_memory_map_t* mmap = (multiboot_memory_map_t*)mboot->mmap_addr;
	char* string;
	while((uint32_t)mmap < mboot->mmap_addr + mboot->mmap_length) {
		mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(unsigned int) );
	}
	klog(LOG_INFO,"x86","Done starting hardware!\n");
	kmain();
}