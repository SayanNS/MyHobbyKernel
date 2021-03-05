#include "idt.h"
#include "memory/memorymanager.h"
#include "common/print.h"

void set_gate_descriptor(GateDescriptor *descriptor, uint16_t offset, void (*handler)(), uint8_t privilege,	uint8_t type)
{
	descriptor->handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
	descriptor->handlerAddressHighBits = ((uint32_t)handler >> 16) & 0xFFFF; 
	descriptor->gdt_codeSegmentSelector = offset;
	descriptor->access = IDT_DESC_PRESENT | type | ((privilege & 3) << 5); 
	descriptor->reserved = 0;
}

struct GateDescriptor * init_interrupt_desctiptor_table()
{
	GateDescriptor *gateDescriptors = (GateDescriptor *) kmalloc(sizeof(struct GateDescriptor) * GATE_DESCRIPTORS_COUNT);
	int offset = SegmentDescriptorIndex::KERNEL_CODE_DESCRIPTOR * sizeof(struct SegmentDescriptor);

	for (int i = 0; i < GATE_DESCRIPTORS_COUNT; i++) {
		set_gate_descriptor(gateDescriptors + i, offset, ignore_interrupt, 0, IDT_INTERRUPT_GATE);
	}

	InterruptDescriptorTablePointer idtp;
	idtp.size = GATE_DESCRIPTORS_COUNT * sizeof(GateDescriptor) - 1;
	idtp.base = (uint32_t)gateDescriptors;
	asm volatile("lidt %0" : : "m" (idtp));

	return gateDescriptors;
}