#include "mm/memorymanager.h"
#include "common/print.h"
#include "panic.h"
 
int alocate_pages(int number)
{
	return 0;
}

void free_pages(int number)
{
	
}

void * kmalloc(int size)
{
	foreach (plist, memory_info.list_head) {
		struct memory_list *memory = container_of(plist, memory_list, list_head);
		if (memory->length >= size) {
			memory->length -= size;
			return (void *)((int) memory + memory->length);
		} else {
			panic("kmalloc not implemented completely");
		}
	}
	panic("kmalloc not implemented completely");
}

void kfree(void *ptr, int size)
{

}


void insert_after(list_head *new_list, list_head *old_list)
{
	new_list->next = old_list->next;
	old_list->next = new_list;
	new_list->prev = old_list;
	new_list->next->prev = new_list;
}

void insert_before(list_head *new_list, list_head *old_list)
{

}