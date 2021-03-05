
#ifndef __TYPES_H
#define __TYPES_H

typedef char int8_t;
typedef unsigned char uint8_t;

typedef short int16_t;
typedef unsigned short uint16_t;

typedef int int32_t;
typedef unsigned int uint32_t;

typedef long long int64_t;
typedef unsigned long long uint64_t;

typedef uint32_t *uintptr_t;

struct list_head
{
	struct list_head *next;
	struct list_head *prev;
};

#define offsetof(st, m) \
	((int)&(((st *)0)->m))

#define container_of(ptr, type, member) \
	((type *)((int) ptr - offsetof(type, member)))

#define foreach(plist, head) \
	for (struct list_head *plist = head.next; plist != &head; plist = plist->next)

#endif