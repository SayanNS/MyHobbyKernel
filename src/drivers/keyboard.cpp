
#include "common/config.h"
#include "drivers/keyboard.h"
#include "common/print.h"
#include "hardwarecommunication/port.h"

#define KEYBOARD_IRQ_NUMBER 1

#define DATA_PORT 0x60
#define COMMAND_PORT 0x64

void handle_interrupt_request0x01();

void init_keyboard(GateDescriptor *idt, int irq_offset)
{
	int offset = SegmentDescriptorIndex::KERNEL_CODE_DESCRIPTOR * sizeof(struct SegmentDescriptor);
	set_gate_descriptor(idt + (irq_offset + KEYBOARD_IRQ_NUMBER), offset, handle_interrupt_request0x01, 0,	IDT_INTERRUPT_GATE);

	while (inb(COMMAND_PORT) & 0x1) {
		inb(DATA_PORT);
	}
	outb(COMMAND_PORT, 0xAE); // activate interrupts
	outb(COMMAND_PORT, 0x20); // get current state
	uint8_t status = (inb(DATA_PORT) | 1) & ~0x10;
	outb(COMMAND_PORT, 0x60); // set state
	outb(DATA_PORT, status);

	outb(DATA_PORT, 0xF4); // activate keyboard (kinda)
}

void handle_interrupt()
{
	uint8_t key = inb(DATA_PORT);
	printf("KEYBOARD %x\n", key);
#ifdef USE_PIC
#include "hardwarecommunication/pic.h"
	PIC_sendEOI(KEYBOARD_IRQ_NUMBER);
#endif
}