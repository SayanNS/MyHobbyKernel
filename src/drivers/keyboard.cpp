
#include "common/config.h"
#include "drivers/keyboard.h"
#include "common/print.h"
#include "hardwarecommunication/port.h"

#define KEYBOARD_IRQ_NUMBER 1

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_COMMAND_PORT 0x64

void init_keyboard()
{
	// while (inb(KEYBOARD_COMMAND_PORT) & 0x1) {
	// 	inb(KEYBOARD_DATA_PORT);
	// }
	// outb(KEYBOARD_COMMAND_PORT, 0xAE);
	// outb(KEYBOARD_COMMAND_PORT, 0x20);
	// uint8_t status = (inb(KEYBOARD_DATA_PORT) | 1) & ~0x10;
	// outb(KEYBOARD_COMMAND_PORT, 0x60);
	// outb(KEYBOARD_DATA_PORT, status);
	// outb(KEYBOARD_DATA_PORT, 0xF4);
}

extern "C" void handle_keyboard_interrupt()
{
	uint8_t key = inb(KEYBOARD_DATA_PORT);
	printf("KEYBOARD %x\n", key);
}