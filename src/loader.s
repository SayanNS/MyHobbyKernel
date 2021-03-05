.section .text
.extern kernel_main
// .extern call_constructors
.global entry_addr

entry_addr:
	mov $kernel_stack, %esp

	// should save ebx and eax before calling constructors
	// call call_constructors

	push %ebx
	push %eax
	call kernel_main
	add $8, %esp

.global _stop
_stop:
	hlt
	jmp _stop

.section .bss
.space 4 * 1024; # 4KB
kernel_stack: