
#ifndef __PIC_H
#define __PIC_H

#include "common/types.h"

void init_pic(int master_offset, int slave_offset);
void PIC_sendEOI(uint8_t irq);

#endif