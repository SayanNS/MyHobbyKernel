
#ifndef __PRINT_H
#define __PRINT_H

#include "common/types.h"

enum Color : uint8_t
{
	BLACK,
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

void printf(const char *format, ...);

#endif