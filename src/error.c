#include <log.h>
#include <error.h>
#include <beryllium/video.h>
#include <stdio.h>
#ifdef X86
#include <x86/low/isr.h>
#include <elf.h>
#endif
#define PANIC_MSG_BAR "   ==================================PANIC===================================   "

#ifdef X86
extern elf_t kernel_elf;

void print_stack_trace ()
{
	uint32_t *ebp, *eip;
	asm volatile ("mov %%ebp, %0" : "=r" (ebp));
	while (ebp)
	{
		eip = ebp+1;
		printf ("| [0x%x]: %s\n", *eip, elf_lookup_symbol (*eip, &kernel_elf));
		ebp = (uint32_t*) *ebp;
	}
}
#endif

void panic(const char* reason)
{
	//terminal_set_statusbar(PANIC_MSG_BAR);
	video_setattributetext(0,0xC);
	#ifdef X86
	printf(PANIC_MSG_BAR);
	#endif
	video_resetattributetext();
	klog(LOG_PANIC,"PANIC",reason);
	halt();
}
void halt()
{
	klog(LOG_INFO,"KERN","Halting!\n");
	#ifdef X86
	printf("Stacktrace:\n");
	print_stack_trace();
	asm("cli");
	asm("hlt");
	#endif
	while(true)
	{
		
	}
}
#ifdef X86
void halt_regs(registers_t* regs)
{
	klog(LOG_INFO,"KERN","Halting!\n");

	#ifdef X86
	asm("cli");
	printf("Registers:\n");
	printf("| eax 0x%X; ebx 0x%X; ecx 0x%X; edx 0x%X\n",regs->eax,regs->ebx,regs->ecx,regs->edx);
	printf("| ??? 0x%X; ebp 0x%X; err 0x%X; efl 0x%X\n",regs->useless_value,regs->ebp,regs->err_code,regs->eflags);
	printf("| usp 0x%X; eip 0x%X; esi 0x%X; edi 0x%X\n",regs->useresp,regs->ebp,regs->esi,regs->edi);
	printf("| cs 0x%X; ds 0x%X; es 0x%X; fs 0x%X\n",regs->cs,regs->ds,regs->es,regs->fs);
	printf("| gs  0x%X\n",regs->gs);
	halt();
	#endif
}
#endif
