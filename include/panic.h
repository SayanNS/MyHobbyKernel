
#ifndef __PANIC_H
#define __PANIC_H

#include "common/print.h"

extern "C" void _stop();

#define panic(args...) \
	printf(args); \
	_stop();

#endif