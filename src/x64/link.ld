ENTRY(bootstrap)
SECTIONS
{
	. = 1M;
   .text : AT(ADDR(.text))
   {
		code = .;
		*(.rodata*)
    	. = ALIGN(4096);
   }

   .data : AT(ADDR(.data))
   {
        data = .;
        *(.data)
        . = ALIGN(4096);
   }

   .eh_frame : AT(ADDR(.eh_frame))
   {
       ehframe = .;
       *(.eh_frame)
        . = ALIGN(4096);
   }

   .bss : AT(ADDR(.bss))
   {
       bss = .;
       *(.bss)

       /*
        * You usually need to include generated COMMON symbols
        * under kernel BSS section or use gcc's -fno-common
        */

        *(COMMON)
       . = ALIGN(4096);
   }

   end = .;

   /DISCARD/ :
   {
        *(.comment)
   }
}