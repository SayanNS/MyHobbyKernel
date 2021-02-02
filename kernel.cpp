#include "types.h"
#include "gdt.h"

struct VideoMemory
{
	uint8_t symbol;
	uint8_t color;
};

void printf(char *str)
{
	struct VideoMemory *VideoMemory = (struct VideoMemory *)0xb8000;

	for (int i = 0; str[i] != '\0'; i++) {
		VideoMemory[i].color = 2;
		VideoMemory[i].symbol = str[i];
	}
}

extern "C" void kernelMain(void *multiboot_structure, uint32_t magicnumber)
{
	printf("Hello World!");
	
	GlobalDescriptorTable gdt;

}
