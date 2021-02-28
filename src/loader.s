.section .text
.extern _Z11kernel_mainjP14multiboot_info
// .extern call_constructors
.global entry_addr

entry_addr:
	mov $kernel_stack, %esp

	// should save ebx and eax before calling constructors
	// call call_constructors

	push %ebx
	push %eax
	call _Z11kernel_mainjP14multiboot_info
	add %esp, 8

_stop:
	cli
	hlt
	jmp _stop

.section .bss
.space 4 * 1024; # 4KB
kernel_stack: