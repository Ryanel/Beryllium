.section .init
.globl _start
_start:
	b enable_led
	b halt
halt:
	b halt

enable_led:
	mov r1,#1
	lsl r1,#18
	str r1,[r0,#4]

	mov r1,#1
	lsl r1,#16
	str r1,[r0,#40]

	b halt
	