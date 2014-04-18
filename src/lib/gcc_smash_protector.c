#include <error.h>
#include <stdio.h>
#include <stdlib.h>
//#include <beryllium/random.h>
void * __stack_chk_guard = NULL;
 
void __stack_chk_guard_setup()
{
    unsigned int * p;
    p = (unsigned int *) &__stack_chk_guard;
 
    /* If you have the ability to generate random numbers in your kernel then use them,
       otherwise for 32-bit code: */
    *p =  0x00000aff;
}
 
void __attribute__((noreturn)) __stack_chk_fail()
{ 
    panic("Stack was smashed!");
    while(true)
    {
	asm("hlt");
    }
    asm("hlt");
}
