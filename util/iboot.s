[ORG 0x7C00]
[BITS 16]
start:
	jmp	0x0000:boot
	times 8-($-$$) db 0

	;	Boot Information Table
	bi_PrimaryVolumeDescriptor  resd  1    ; LBA of the Primary Volume Descriptor
	bi_BootFileLocation         resd  1    ; LBA of the Boot File
	bi_BootFileLength           resd  1    ; Length of the boot file in bytes
	bi_Checksum                 resd  1    ; 32 bit checksum
	bi_Reserved                 resb  40   ; Reserved 'for future standardization'

%macro BiosPrint 1
                mov si, word %1
ch_loop:lodsb
   or al, al
   jz done
   mov ah, 0x0E
   int 0x10
   jmp ch_loop
done:
%endmacro

boot:
	xor ax, ax  ;make it zero
	mov ds, ax
	BiosPrint msg

msg db 'Welcome to Macintosh', 13, 10, 0
