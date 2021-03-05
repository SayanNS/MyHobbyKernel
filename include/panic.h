
#ifndef __PANIC_H
#define __PANIC_H

#include "common/print.h"
#include "idt.h"

extern "C" void _stop();

#define panic(args...) \
	printf(args); \
	deactivate_interrupts() \
	_stop();

#endif