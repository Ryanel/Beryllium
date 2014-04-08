#include <boot/kmain.h>
#include <boot/multiboot.h>

#include <terminal.h>
#include <stdio.h>
#include <log.h>

#include <x86/x86.h>
#include <x86/drivers/serial.h>
#include <elf.h>

uint32_t module_start[256];
uint32_t module_end[256];
#ifdef ENABLE_SERIAL
void serial_print_header();
#endif
extern uint32_t placement_address;
extern elf_t kernel_elf;
void boop()
{

}
/**
 Get the kernel up and running as fast as possible
**/
void kernel_x86_binding_init(int magic,struct multiboot *mboot)
{

	#ifdef IS_RELEASE
	klog_setmask(LOG_INFO);
	#else
	klog_setmask(LOG_DEBUG);
	#endif
	terminal_init();

	#ifdef ENABLE_SERIAL
	serial_init();
	serial_print_header("Intel Compatible","i586");
	#endif
	
	klog(LOG_INFO,"x86","Loading x86 components...\n");
	
	if(magic!=0x2BADB002)
	{
		klog(LOG_PANIC,"MBT","Multiboot Magic number: 0x%X! It should equal 0x2BADB002. Halting...\n",magic);
		return;
	}
	//Set kernel properties from multiboot info.
	klog(LOG_INFO,"MBT","%d modules loaded with kernel\n",mboot->mods_count);
	if(mboot->mods_count != 0)
	{
		module_start[0] = *((uint32_t*)mboot->mods_addr);
		module_end[0] = *(uint32_t*)(mboot->mods_addr+4);
		placement_address = module_end[0];
		klog(LOG_INFO,"MBT","Placement address is now 0x%X\n",placement_address);
		char *tmp;
		char *str1, *token, *subtoken;
		char *saveptr1, *saveptr2;
		int j = 1;
		for (j = 1, str1 = module_start[0]; ; j++, str1 = 0)
		{
			token = strtok_r(str1, "\n", &saveptr1);
			if (token == NULL)
				break;
		}
		klog(LOG_INFO,"SYM","%d symbols loaded\n",j);

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
	while((uint32_t)mmap < mboot->mmap_addr + mboot->mmap_length) {
		mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(unsigned int) );
	}

	klog(LOG_INFO,"x86","Done starting hardware!\n");

	kmain();
}
