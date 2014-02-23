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

daps:
	db	0x0
	db	0
blkcnt:	dw	2		; int 13 resets this to # of blocks actually read/written
db_add:	dw	0x9000	; memory buffer destination address(0:7c00)
	dw	0		; in memory page zero
d_lba:  resd 1			; put the lba to read in this spot
	dd	0		; more storage bytes only for big lbas ( > 4 bytes )

boot:
	mov si, startmsg
	call bios_print

	mov si, mediummsg
	call bios_print

	;Enable A20
	mov ax, 0x2401
	int 0x15
	jc a20err

	mov si, a20msg
	call bios_print

	;Start Loading the ISO stuff
	mov ah, 0x41
	mov bx, 0x55AA
	int 0x13
	jnc int13ex
noint13ex:
	mov si, noi13supportmsg
	call bios_print
	jmp halt
int13ex:
	mov si, i13supportmsg
	call bios_print
boot2:
	mov cx, [bi_PrimaryVolumeDescriptor]
	mov [d_lba], cx
	mov si, daps		; address of "disk address packet"
	mov ah, 0x42		; AL is unused
	int 0x13
	jc readfailed

	xor ax, ax  ;make it zero
	mov gs, ax

	movb  cx, [0x9000]
	cmp cx, 0x10
	je boot3
	jmp nopvd
boot3:

;Functions and such

halt:
	hlt
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

;Errors

a20err:
	mov si, noa20msg
	call bios_print
	jmp halt

readfailed:
	mov si, readfailedmsg
	call bios_print
	jmp halt

nopvd:
	mov si, nopvdmsg
	call bios_print
	jmp halt

;Messages
startmsg db 13, 10, '[iboot]: Loading iboot and kernel...', 13, 10, 0
mediummsg db '[iboot]: Medium is a CDROM', 13, 10, 0


;a20 stuff
a20msg db '[iboot]: A20 gate enabled!', 13, 10, 0
noa20msg db '[iboot]: Unable to enable A20 gate! Halting...', 13, 10, 0


;int 0x13 extentions
i13supportmsg db '[iboot]: int 0x13 extentions supported', 13, 10, 0
noi13supportmsg db '[iboot]: No int 0x13 extentions support, Halting...', 13, 10, 0

;Errors
nopvdmsg db '[iboot]: Could not find Primary Volume Descriptor at 0x8000. Halting...', 13, 10, 0
readfailedmsg db '[iboot]: Reading Failed (in bios!). Halting...', 13, 10, 0

;Reserved space