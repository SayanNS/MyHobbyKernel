
#ifndef __PRINT_H
#define __PRINT_H

#define CONSOLE_WIDTH 80
#define CONSOLE_HEIGHT 25

#include "common/types.h"

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

extern struct VideoMemory *VideoMemory;
extern uint8_t x, y;

void printf(const char *format, ...);

#endif