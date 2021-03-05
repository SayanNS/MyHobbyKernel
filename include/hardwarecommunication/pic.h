
#ifndef __PIC_H
#define __PIC_H

#include "common/types.h"
#include "idt.h"

enum PIC_DEVICE_IRQ_NUMBER
{
	PIC_TIMER,
	PIC_KEYBOARD
};

void init_pic(GateDescriptor *idt, int master_offset, int slave_offset);

extern "C" void pic_timer_interrupt_handler();
extern "C" void pic_keyboard_interrupt_handler();

#endif