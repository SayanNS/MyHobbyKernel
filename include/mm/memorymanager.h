
#ifndef __MEMORY_MANAGER_H
#define __MEMORY_MANAGER_H

#include "common/types.h"

#define PAGE_SIZE 4096
#define BITS sizeof(int) * 8

// extern "C" uint bitmask_location[];

// for now it's a constant but maybe later will change it with a variable
#define bitmask_location ((int *)0x0)

int allocate_pages(int number);
void free_pages(int number);

void *malloc(int size);
void free(void *);

#endif