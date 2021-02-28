#include "multibootheader.h"
#include "common/types.h"
#include "common/print.h"
#include "gdt.h"
#include "hardwarecommunication/interrupts.h"
#include "drivers/keyboard.h"
#include "drivers/mouse.h"
#include "drivers/driver.h"
#include "drivers/vga.h"
#include "drivers/ata.h"
#include "mm/memorymanager.h"

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

static inline void alloc_page_bitmask(uint64_t address, uint64_t length)
{
	if ((address & (PAGE_SIZE - 1)) || (length & (PAGE_SIZE - 1))) {
		printf("memory area: %r:%r - is not page aligned.\n", address, length);
		return;
	}

	address /= PAGE_SIZE;
	length /= PAGE_SIZE;

	int num = address & (BITS - 1); // five/six last bits depending on 32/64 bit mode
	uint64_t i = address / BITS;
	if (num != 0) {
		if (length < 32 && (num + (uint)length <= 32)) {
			bitmask_location[i] = (bitmask_location[i] & ((1 << num) - 1))
								| (((1 << (uint)length) - 1) << num)
								| (bitmask_location[i] & ((1 << (BITS - ((uint)length + num)) - 1) << ((uint)length + num)));
			return;
		}

		bitmask_location[i] = (bitmask_location[i] & ((1 << num) - 1))
							| (((1 << (BITS - num)) - 1) << num);
		
		i++;
		length -= BITS - num;
	}
	
	for (; length >= BITS; i++, length -= BITS) {
		bitmask_location[i] = (uint)-1;
	}

	if (length > 0) {
		bitmask_location[i] = ((1 << (uint)length) - 1)
							| (bitmask_location[i] & ((1 << (BITS - (uint)length) - 1) << (uint)length));
	}
}

void kernel_main(uint32_t mboot_magic, multiboot_info_t *mboot_info)
{
	printf("%x", *(uint32_t *) 0x100000);
	return;
	
	if (mboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		printf("multiboot_magic is not valid: %x != %x", mboot_magic, MULTIBOOT_BOOTLOADER_MAGIC);
		return;
	}

	if ((mboot_info->flags & (1 << 6)) == 0) {
		printf("multiboot_memory_map is invalid");
	} else {
		for (multiboot_memory_map_t *mmap = (multiboot_memory_map_t *) mboot_info->mmap_addr;
			(unsigned long) mmap < mboot_info->mmap_addr + mboot_info->mmap_length;
			mmap = (multiboot_memory_map_t *) ((unsigned long)mmap + mmap->size + sizeof (mmap->size))) {

			if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
				alloc_page_bitmask(mmap->addr, mmap->len);
			}
		}
	}
	
	// GlobalDescriptorTable gdt;
	// InterruptManager interrupts(&gdt);

	// DriverManager driverManager;

	// KeyboardDriver keyboardDriver(&interrupts);

	// MouseDriver mouseDriver(&interrupts);
	// driverManager.AddDriver(mouseDriver);
	// VideoGraphicsArray vga;

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
