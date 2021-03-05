#include "common/config.h"
#include "multibootheader.h"
#include "common/types.h"
#include "common/print.h"
#include "gdt.h"
#include "idt.h"
#include "drivers/keyboard.h"
// #include "drivers/mouse.h"
// #include "drivers/driver.h"
// #include "drivers/vga.h"
// #include "drivers/ata.h"
#include "mm/memorymanager.h"
#include "panic.h"
#include "hardwarecommunication/pic.h"

#define IRQ_OFFSET 0x20

static inline void init_memory_manager(uint32_t, uint32_t);

extern "C" void kernel_main(uint32_t mboot_magic, multiboot_info_t *mboot_info)
{
	if (mboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		panic("multiboot_magic is not valid: %x != %x", mboot_magic, MULTIBOOT_BOOTLOADER_MAGIC);
	}

	if ((mboot_info->flags & (1 << 6)) == 0) {
		panic("multiboot_memory_map is invalid\n");
	}

	init_memory_manager(mboot_info->mmap_addr, mboot_info->mmap_length);
	SegmentDescriptor *gdt = init_global_descriptor_table();
	GateDescriptor *idt = init_interrupt_desctiptor_table();
	
	init_pic(IRQ_OFFSET, IRQ_OFFSET + 8);
	init_keyboard(idt, IRQ_OFFSET);

	printf("activate interrupts\n");
	activate_interrupts();

	for (int i = 0; true; i++) {
		if (i == 0) {
			printf("loop");
		}
	}

	// foreach (plist, memory_info.list_head) {
	// 	printf("%x", container_of(plist, memory_list, list_head)->length);
	// }
	

	// // MouseDriver mouseDriver(&interrupts);
	// // driverManager.AddDriver(mouseDriver);
	// // VideoGraphicsArray vga;

	// interrupts.Activate();

	// vga.SetMode(320, 200, 8);
	// for (int32_t y = 0; y < 200; y++) {
	// 	for (int32_t x = 0; x < 320; x++) {
	// 		vga.PutPixel(x, y, 0, 0, 0);
	// 	}
	// }

	// interrupt 14
	// ATA ata0m(0x1f0, true);
	// ATA ata0s(0x1f0, false);

	// // interrupt 15
	// ATA ata1m(0x170, true);
	// ATA ata1s(0x170, false);
}

extern "C" uint32_t header_addr;
extern "C" uint32_t load_addr;
extern "C" uint32_t load_end_addr;
extern "C" uint32_t bss_end_addr;
extern "C" uint32_t entry_addr;

struct multiboot_header static const multiboot_header __attribute__((section(".multiboot"))) = {
	.magic = MULTIBOOT_HEADER_MAGIC,
	.flags = MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE,
	.checksum = -(multiboot_header.magic + multiboot_header.flags),
	.header_addr = (uint32_t) &header_addr,
	.load_addr = (uint32_t) &load_addr,
	.load_end_addr = (uint32_t) &load_end_addr,
	.bss_end_addr = (uint32_t) &bss_end_addr,
	.entry_addr = (uint32_t) &entry_addr
};

static void alloc_page_bitmask(int page, int length)
{
	int num = page % BITS; // five/six last bits depending on 32/64 bit mode
	int i = page / BITS;
	if (num != 0) {
		if (length < BITS && (num + (int)length <= BITS)) {
			bitmask_location[i] = (bitmask_location[i] & ((1 << num) - 1))
								| (((1 << (int)length) - 1) << num)
								| (bitmask_location[i] & ((1 << (BITS - ((int)length + num)) - 1) << ((int)length + num)));
			return;
		}

		bitmask_location[i] = (bitmask_location[i] & ((1 << num) - 1))
							| (((1 << (BITS - num)) - 1) << num);
		
		i++;
		length -= BITS - num;
	}
	
	for (; length >= BITS; i++, length -= BITS) {
		bitmask_location[i] = (int)-1;
	}

	if (length > 0) {
		bitmask_location[i] = ((1 << (int)length) - 1)
							| (bitmask_location[i] & ((1 << (BITS - (int)length) - 1) << (int)length));
	}
}

/*
 * TODO: check this function again later
 */
static void fill_area_info(uint64_t address, uint64_t length)
{
#ifdef MODE_32
	if ((address >> 32) > 0) {
		printf("memory area: %r %r - is higher than 4GB\n", address, length);
		return;
	}
#endif
	if ((int) address % PAGE_SIZE != 0) {
		printf("Label is not impelemented file: %s, line: %d\n", __FILE__, __LINE__);
		address &= ~(PAGE_SIZE - 1);
	}
	memory_info.area_info[memory_info.area_info_count].page = ((int) address) / PAGE_SIZE;
#ifdef MODE_32
	if (((address + length) >> 32) > 0) {
		/*
		 * FIX: If page equals to zero than length also becomes zero
		 */
		memory_info.area_info[memory_info.area_info_count].length = -memory_info.area_info[memory_info.area_info_count].page;
		memory_info.area_info_count++;
		return;
	}
#endif
	if ((int) length % PAGE_SIZE != 0) {
		struct memory_list *memory = (struct memory_list *)((int) address + (int) length & ~(PAGE_SIZE - 1));
		memory->length = (int) length % PAGE_SIZE;
		insert_after(&memory->list_head, &memory_info.list_head);
	}
	memory_info.area_info[memory_info.area_info_count].length = ((int) length) / PAGE_SIZE;
	memory_info.area_info_count++;
}

static inline void init_memory_manager(uint32_t mmap_addr, uint32_t mmap_length)
{
	memory_info.list_head.next = memory_info.list_head.prev = &memory_info.list_head;

	uint64_t available_memory_addr = 0;
	uint64_t available_memory_len = 0;
	memory_info.area_info_count = 0;

	for (multiboot_memory_map_t *mmap = (multiboot_memory_map_t *) mmap_addr;
		(unsigned long) mmap < mmap_addr + mmap_length;
		mmap = (multiboot_memory_map_t *) ((unsigned long)mmap + mmap->size + sizeof (mmap->size))) {
		
		if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
			if (available_memory_addr + available_memory_len == mmap->addr) {
				available_memory_len += mmap->len;
			} else {
				if (available_memory_len > 0) {
					fill_area_info(available_memory_addr, available_memory_len);
				}
				available_memory_addr = mmap->addr;
				available_memory_len = mmap->len;
			}
		}
	}
	if (available_memory_len > 0) {
		fill_area_info(available_memory_addr, available_memory_len);
		/*
		 * maybe that's a bad idea to put bitmask right after memory_info structure 
		 * I'm going to consider putting bitmask to another location later
		 */
		memory_info.page_bitmask = (int *)((int)&memory_info + sizeof(memory_info) + sizeof(area_info) * memory_info.area_info_count);
	}

	for (int i = 0; i < memory_info.area_info_count; i++) {
		/*
		 * TODO: set page bits where bitmask and os reside
		 */
		alloc_page_bitmask(memory_info.area_info[i].page, memory_info.area_info[i].length);
	}
}