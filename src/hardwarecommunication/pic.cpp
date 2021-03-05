
#include "hardwarecommunication/pic.h"
#include "hardwarecommunication/port.h"

#define PIC_MASTER_COMMAND_PORT 0x20
#define PIC_SLAVE_COMMAND_PORT 0xA0
#define PIC_MASTER_DATA_PORT 0x21
#define PIC_SLAVE_DATA_PORT 0xA1

#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
 
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

void init_pic(GateDescriptor *idt, int master_offset, int slave_offset)
{
	uint8_t master_mask, slave_mask;

	master_mask = inb(PIC_MASTER_DATA_PORT);
	slave_mask = inb(PIC_SLAVE_DATA_PORT);

	outb(PIC_MASTER_COMMAND_PORT, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC_SLAVE_COMMAND_PORT, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC_MASTER_DATA_PORT, master_offset);
	io_wait();
	outb(PIC_SLAVE_DATA_PORT, slave_offset);
	io_wait();
	outb(PIC_MASTER_DATA_PORT, 4);
	io_wait();
	outb(PIC_SLAVE_DATA_PORT, 2);
	io_wait();

	outb(PIC_MASTER_DATA_PORT, ICW4_8086);
	io_wait();
	outb(PIC_SLAVE_DATA_PORT, ICW4_8086);
	io_wait();

	outb(PIC_MASTER_DATA_PORT, master_mask);
	outb(PIC_SLAVE_DATA_PORT, slave_mask);

	int offset = SegmentDescriptorIndex::KERNEL_CODE_DESCRIPTOR * sizeof(struct SegmentDescriptor);
	set_gate_descriptor(idt + master_offset + PIC_DEVICE_IRQ_NUMBER::PIC_TIMER, offset, pic_timer_interrupt_handler, 0, IDT_INTERRUPT_GATE);
	set_gate_descriptor(idt + master_offset + PIC_DEVICE_IRQ_NUMBER::PIC_KEYBOARD, offset, pic_keyboard_interrupt_handler, 0, IDT_INTERRUPT_GATE);
}

// void PIC_sendEOI(uint8_t irq)
// {
// 	if (irq > 8) {
// 		outb(PIC_SLAVE_COMMAND_PORT, PIC_EOI);
// 	}
// 	outb(PIC_MASTER_COMMAND_PORT, PIC_EOI);
// }