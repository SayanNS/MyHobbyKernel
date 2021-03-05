#include "idt.h"
#include "mm/memorymanager.h"
#include "common/print.h"

// uint32_t InterruptManager::HandleInterrupt(uint8_t interruptNumber, uint32_t esp)
// {
// 	if (ActiveInterruptManager != 0) {
// 		return ActiveInterruptManager->DoHandleInterrupt(interruptNumber, esp);
// 	}

// 	return esp;
// }

// uint32_t InterruptManager::DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp)
// {
// 	if (handlers[interruptNumber] != 0) {
// 		esp = handlers[interruptNumber]->HandleInterrupt(esp);
// 	} else if (interruptNumber != 0x20) {
// 		char foo[] = "UNHANDLED INTERRUPT 0x00\n";
// 		char hex[] = "0123456789ABCDEF";
// 		foo[22] = hex[(interruptNumber >> 4) & 0x0F];
// 		foo[23] = hex[interruptNumber & 0x0F];
// 		printf(foo);
// 	}

void ignore_interrupt();

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

#include "drivers/keyboard.h"

void handle_interrupt_request(int number)
{
	if (number == 0x21) {
		handle_interrupt();
	}
	else {
		printf("INTERRUPT %x\n", number);
	}
}
