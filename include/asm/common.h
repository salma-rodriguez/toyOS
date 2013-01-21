#ifndef _COMMON_H_
#define _COMMON_H_

#include <kernel/types.h>

__u8 inportb(__u16 port);
__u16 inportw(__u16 port);
void outportb(__u16 port, __u8 value);

static inline void enable_interrupts()
{
	__asm__ __volatile__("sti");
}

static inline void disable_interrupts()
{
	__asm__ __volatile__("cli");
}

static inline void start_system_timer()
{
	__asm__ __volatile__("int $0x08");
}

#endif /* _COMMON_H_*/
