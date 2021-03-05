
#ifndef __MEMORY_MANAGER_H
#define __MEMORY_MANAGER_H

#include "common/types.h"

#define PAGE_SIZE 4096
#define BITS (sizeof(int) * 8)

extern "C" struct memory_info memory_info;
#define bitmask_location memory_info.page_bitmask

struct area_info
{
	int page;
	int length;
};

struct memory_info
{
	struct list_head list_head;
	int *page_bitmask;
	int area_info_count;
	struct area_info area_info[];
};

struct memory_list 
{
	struct list_head list_head;
	int length;
};

int allocate_pages(int number);
void free_pages(int number);

void *kmalloc(int size);
void kfree(void *ptr, int size);

void insert_after(list_head *new_list, list_head *old_list);
void insert_before(list_head *new_list, list_head *old_list);

#endif