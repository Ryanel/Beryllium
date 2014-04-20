[BITS 64]
section .text
[GLOBAL start]

align 4
stack_bottom:
times 16384 db 0
stack_top:
	
start:
	mov rsp, stack_top
	mov rbp, 0
	push rbx
	push rax
	cli
	;call kernel_x86_binding_init
	pop rax
	pop rbx
	jmp $