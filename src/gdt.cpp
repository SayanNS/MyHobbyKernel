
#include "gdt.h"
#include "mm/memorymanager.h"
#include "common/print.h"

static void init_segment_descriptor(struct SegmentDescriptor *segment_descriptor, uint32_t base, uint32_t limit, uint8_t flags)
{
	uint8_t *target = (uint8_t *)segment_descriptor;

	if (limit < 0x10000) {
		target[6]= 0x40;
	} else {
		if ((limit & 0xFFF) != 0XFFF)
			limit = (limit >> 12) - 1;
		else
			limit = limit >> 12;

		target[6] = 0xC0;
	}

	target[0] = limit & 0xFF;
	target[1] = (limit >> 8) & 0xFF;
	target[6] |= (limit >> 16) & 0xF;

	target[2] = (base) & 0xFF;
	target[3] = (base >> 8) & 0xFF;
	target[4] = (base >> 16) & 0xFF;
	target[7] = (base >> 24) & 0xFF;

	target[5] = flags;
}

SegmentDescriptor * init_global_descriptor_table()
{
	SegmentDescriptor *segmentDescriptors = (SegmentDescriptor *) kmalloc(sizeof(SegmentDescriptor) * SEGMENT_DESCRIPTORS_COUNT);
	init_segment_descriptor(segmentDescriptors + SegmentDescriptorIndex::NULL_SEGMENT_DESCRIPTOR, 0, 0, 0);
	init_segment_descriptor(segmentDescriptors + SegmentDescriptorIndex::UNUSED_SEGMENT_DESCRIPTOR, 0, 0, 0);
	init_segment_descriptor(segmentDescriptors + SegmentDescriptorIndex::KERNEL_CODE_DESCRIPTOR, 0, 64 * 1024 * 1024, 0x9A);
	init_segment_descriptor(segmentDescriptors + SegmentDescriptorIndex::KERNEL_DATA_DESCRIPTOR, 0, 64 * 1024 * 1024, 0x92);


	GlobalDescriptorTablePointer gdtp;
	gdtp.size = SEGMENT_DESCRIPTORS_COUNT * sizeof(SegmentDescriptor) - 1;
	gdtp.base = (uint32_t) segmentDescriptors;
	asm volatile("lgdt %0": : "m" (gdtp));

	// uint32_t i[2];
	// i[1] = (uint32_t) gdt;
	// i[0] = sizeof(struct GlobalDescriptorTable) << 16;
	// asm volatile("lgdt (%0"): : "m" ((uint32_t*) i + 2));
	return segmentDescriptors;
}

uint32_t get_base(struct SegmentDescriptor *segment_descriptor)
{
	uint8_t *target = (uint8_t *)segment_descriptor;
	uint32_t result = target[7];
	result = (result << 8) + target[4];
	result = (result << 8) + target[3];
	result = (result << 8) + target[2];

	return result;
}

uint32_t get_limit(struct SegmentDescriptor *segment_descriptor)
{
	uint8_t *target = (uint8_t *)segment_descriptor;
	uint32_t result = target[6] & 0xF;
	result = (result << 8) + target[1];
	result = (result << 8) + target[0];

	if ((target[6] & 0xC0) == 0xC0) {
		result = (result << 12) | 0xFFF;
	}

	return result;
}