[ORG 0x7C00]
[BITS 16]
start:
	jmp 0x0000:boot
	times 8-($-$$) db 0

; Boot Information Table
bi_PrimaryVolumeDescriptor  resd  1    ; LBA of the Primary Volume Descriptor
bi_BootFileLocation         resd  1    ; LBA of the Boot File
bi_BootFileLength           resd  1    ; Length of the boot file in bytes
bi_Checksum                 resd  1    ; 32 bit checksum
bi_Reserved                 resb  40   ; Reserved 'for future standardization'
boot:
	mov si, startmsg
	call bios_print

	mov si, mediummsg
	call bios_print

	;Start Loading the ISO stuff
	mov ah, 0x41
	mov bx, 0x55AA
	int 0x13
	jc noint13ex
	jmp int13ex
noint13ex:
	mov si, noi13supportmsg
	call bios_print
	jmp halt
int13ex:
	mov si, i13supportmsg
	call bios_print
halt:
	jmp halt

bios_print:
	xor ax, ax  ;make it zero
	mov ds, ax
bios_print_loop:
	lodsb
	or al, al  ;zero=end of str
	jz done    ;get out
	mov ah, 0x0E
	int 0x10
	jmp bios_print_loop
done:
	ret

startmsg db '[iboot]: Loading...', 13, 10, 0
mediummsg db '[iboot]: Medium is CDROM', 13, 10, 0

;int 0x13 extentions
i13supportmsg db '[iboot]: int 0x13 supported', 13, 10, 0
noi13supportmsg db '[iboot]: No int 0x13 support, halting...', 13, 10, 0