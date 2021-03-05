#ifndef __GDT_H
#define __GDT_H

#include "common/types.h"

#define SEGMENT_DESCRIPTORS_COUNT 4

enum SegmentDescriptorIndex
{
	NULL_SEGMENT_DESCRIPTOR,
	UNUSED_SEGMENT_DESCRIPTOR,
	KERNEL_CODE_DESCRIPTOR,
	KERNEL_DATA_DESCRIPTOR,
	USER_CODE,
	USER_DATA
};

struct SegmentDescriptor
{
	uint16_t limit_lo;
	uint16_t base_lo;
	uint8_t base_hi;
	uint8_t type;
	uint8_t flags_limit_hi;
	uint8_t base_vhi;
} __attribute__((packed));

struct GlobalDescriptorTablePointer
{
	uint16_t size;
	uint32_t base;
} __attribute__((packed));

SegmentDescriptor * init_global_descriptor_table();

#endif