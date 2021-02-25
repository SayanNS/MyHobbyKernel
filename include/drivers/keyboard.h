
#ifndef __KEYBOARD_H
#define __KEYBOADR_H

#include "common/types.h"
#include "hardwarecommunication/interrupts.h"
#include "hardwarecommunication/port.h"

class KeyboardDriver : public InterruptHandler
{
	Port8Bit dataport;
	Port8Bit commandport;
public:
	KeyboardDriver(InterruptManager *interrupManager);
	~KeyboardDriver();
	virtual uint32_t HandleInterrupt(uint32_t esp);
};

#endif