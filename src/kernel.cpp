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

struct multiboot_header const multiboot_header __attribute__((section(".multiboot"))) = {
	.magic = MULTIBOOT_HEADER_MAGIC,
	.flags = MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO,
	.checksum = -(multiboot_header.magic + multiboot_header.flags)
};

// typedef void (*constructor)();
// extern "C" constructor start_ctors;
// extern "C" constructor end_ctors;
// extern "C" void call_constructors()
// {
// 	for (constructor *i = &start_ctors; i != &end_ctors; i++) {
// 		(*i)();
// 	}
// }

extern "C" void kernel_main(uint32_t mboot_magic, void *mboot_header)
{
	if (mboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		printf("ERROR");
	}

	multiboot_info_t *mboot_info = (multiboot_info_t *)mboot_header;

	if ((mboot_info->flags & (1 << 6)) == 0) {
		printf("error");
	}

	for (multiboot_memory_map_t *mmap = (multiboot_memory_map_t *) mboot_info->mmap_addr;
		(unsigned long) mmap < mboot_info->mmap_addr + mboot_info->mmap_length;
		mmap = (multiboot_memory_map_t *) ((unsigned long)mmap + mmap->size + sizeof (mmap->size))) {
		printf (" size = 0x%x, base_addr = 0x%r,"
				" length = 0x%r, type = 0x%x\n",
				mmap->size,
				mmap->addr,
				mmap->len,
				mmap->type);
	}
	
		
	GlobalDescriptorTable gdt;
	InterruptManager interrupts(&gdt);

	DriverManager driverManager;

	KeyboardDriver keyboardDriver(&interrupts);

	// MouseDriver mouseDriver(&interrupts);
	// driverManager.AddDriver(mouseDriver);
	// VideoGraphicsArray vga;

	interrupts.Activate();

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
