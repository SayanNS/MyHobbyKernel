.set IRQ_BASE, 0x20
.section .text

// .macro HandleException num
// .global _ZN16InterruptManager16handleException\num\()Ev
// _ZN16InterruptManager16handleException\num\()Ev:
// 	movb $\num, (interruptnumber)
// 	jmp int_bottom
// .endm

.macro handle_interrupt_request num
.global _Z28handle_interrupt_request\num\()v
_Z28handle_interrupt_request\num\()v:
	movl $\num + IRQ_BASE, (interruptnumber)
	jmp int_bottom
.endm

// HandleInterruptRequest 0x00
handle_interrupt_request 0x01
// HandleInterruptRequest 0x0C

int_bottom:
    push %ebp
    push %edi
    push %esi

    push %edx
    push %ecx
    push %ebx
    push %eax

	pushl (interruptnumber)
	call _Z24handle_interrupt_requesti
	add $4, %esp

    pop %eax
    pop %ebx
    pop %ecx
    pop %edx

    pop %esi
    pop %edi
    pop %ebp

.global _Z16ignore_interruptv
_Z16ignore_interruptv:
	iret

.data
	interruptnumber: .long 0
