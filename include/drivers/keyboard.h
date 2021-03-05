
#ifndef __KEYBOARD_H
#define __KEYBOADR_H

#include "common/types.h"
#include "idt.h"

void init_keyboard(GateDescriptor *idt, int irq_offset);
void handle_interrupt();

#endif