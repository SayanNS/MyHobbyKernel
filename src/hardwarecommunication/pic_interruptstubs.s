.section .text

.global pic_timer_interrupt_handler
pic_timer_interrupt_handler:

	push %eax
	jmp pic_master_eoi

.global pic_keyboard_interrupt_handler
pic_keyboard_interrupt_handler:

	push %eax
	push %edx
	push %ecx

	call handle_keyboard_interrupt

	pop %ecx
	pop %edx

	jmp pic_master_eoi

pic_slave_eoi:
	mov $0x20, %al
	outb %al, $0xA0
	outb %al, $0x20
	jmp ret
pic_master_eoi:
	mov $0x20, %al
	outb %al, $0x20
ret:
	pop %eax
	iret
