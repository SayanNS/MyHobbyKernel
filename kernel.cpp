#include "types.h"
#include "gdt.h"
#include "interrupts.h"
#include "keyboard.h"
#include "mouse.h"

#define CONSOLE_WIDTH 80
#define CONSOLE_HEIGHT 25

enum Color : uint8_t
{
	BLACK = 0,
	MAROON,
	GREEN,
	OLIVE,
	NAVY,
	PURPLE,
	TEAL,
	SILVER,
	GRAY,
	RED,
	LIME,
	YELLOW,
	BLUE,
	FUCHSIA,
	AQUA,
	WHITE
};

struct VideoMemory
{
	uint8_t symbol;
	Color color;
};

void printf(char *str)
{
	static struct VideoMemory *VideoMemory = (struct VideoMemory *)0xb8000;
	static uint8_t x = 0, y = 0;

	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] == '\n') {
			x = 0;
			if (++y == CONSOLE_HEIGHT)
				y = 0;
		} else {
			VideoMemory[CONSOLE_WIDTH * y + x].color = Color::SILVER;
			VideoMemory[CONSOLE_WIDTH * y + x].symbol = str[i];
			if (++x == CONSOLE_WIDTH) {
				x = 0;
				if (++y == CONSOLE_HEIGHT)
					y = 0;
			}
		}
	}
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
	for (constructor *i = &start_ctors; i != &end_ctors; i++) {
		(*i)();
		printf("Call\n");
	}
}

extern "C" void kernelMain(void *multiboot_structure, uint32_t magicnumber)
{
	printf("Hello World!\n");

	GlobalDescriptorTable gdt;
	InterruptManager interrupts(&gdt);
	KeyboardDriver keyboardDriver(&interrupts);
	MouseDriver mouseDriver(&interrupts);
	interrupts.Activate();

	while (1);
}
