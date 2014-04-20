;
; boot.s
; The entry point of the kernel
; By (as in edited by) Corwin Mcknight -- Credit to James Molloy (jamesmolloy.co.uk)
;
[BITS 32]                       ; All instructions should be 32-bit.
[GLOBAL multiboot]              ; Make 'mboot' accessible from C.
[EXTERN code]                   ; Start of the '.text' section.
[EXTERN bss]                    ; Start of the .bss section.
[EXTERN end]                    ; End of the last loadable section.

MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1    ; Provide your kernel with memory info
MBOOT_VIDEO_MODE    equ 1<<2    ; Provide your kernel with memory info
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic value
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO | MBOOT_VIDEO_MODE
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

section .text

align 4
multiboot:
	dd  MBOOT_HEADER_MAGIC        ; GRUB will search for this value on each
								  ; 4-byte boundary in your kernel file
	dd  MBOOT_HEADER_FLAGS        ; How GRUB should load your file / settings
	dd  MBOOT_CHECKSUM            ; To ensure that the above values are correct

	dd  multiboot                 ; Location of this descriptor
	dd  code                      ; Start of kernel '.text' (code) section.
	dd  bss                       ; End of kernel '.data' section.
	dd  end                       ; End of kernel.
	dd  start                     ; Kernel entry point (initial EIP).
[EXTERN start]
[EXTERN x86_64_early]
;[EXTERN kernel_x86_binding_init]
[global bootstrap]
bootstrap:
	mov word [0xb8000], 0x07690748
	jmp x86_64_early
    ;(32-bit to 64-bit code goes here)
    ;(jump to 64-bit code)
	;jmp strt