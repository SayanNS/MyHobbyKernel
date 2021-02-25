.section .text
.extern kernel_main
.extern call_constructors
.global loader

loader:
	mov $kernel_stack, %esp

	// should save ebx and eax before calling constructors
	// call call_constructors

	push %ebx
	push %eax
	call kernel_main
	add %esp, 8

_stop:
	cli
	hlt
	jmp _stop

.section .bss
.space 2 * 1024 * 1024; # 2 MiB
kernel_stack:
