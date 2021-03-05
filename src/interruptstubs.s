.section .text

// int_bottom:
// 	push %edx
// 	push %ecx
// 	push %eax

// 	call _Z24handle_interrupt_requesti

// 	pop %eax
// 	pop %ecx
// 	pop %edx

.global _Z16ignore_interruptv
_Z16ignore_interruptv:
	iret
