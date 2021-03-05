
#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include "common/types.h"
#include "gdt.h"

#define GATE_DESCRIPTORS_COUNT 256
#define IDT_INTERRUPT_GATE 0xE
#define IDT_DESC_PRESENT 0x80

struct GateDescriptor
{
	uint16_t handlerAddressLowBits;
	uint16_t gdt_codeSegmentSelector;
	uint8_t reserved;
	uint8_t access;
	uint16_t handlerAddressHighBits;
} __attribute__((packed));

struct InterruptDescriptorTablePointer
{
	uint16_t size;
	uint32_t base;
} __attribute__((packed));

GateDescriptor * init_interrupt_desctiptor_table();

static inline void activate_interrupts()
{
	asm("sti");
}

void set_gate_descriptor(GateDescriptor *descriptor, uint16_t offset, void (*handler)(), uint8_t privilege,	uint8_t type);

#endif