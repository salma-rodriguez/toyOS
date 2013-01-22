#ifndef _COMMON_H_
#define _COMMON_H_

#include <kernel/types.h>

#define EOI 0x20

#define PICS 0xA0
#define PICM 0x20

__u8 inportb(__u16 port);
__u16 inportw(__u16 port);
void outportb(__u16 port, __u8 value);

static inline void do_eoi()
{
	outportb(PICM, EOI);
}

static inline void enable_interrupts()
{
	__asm__ __volatile__("sti");
}

static inline void disable_interrupts()
{
	__asm__ __volatile__("cli");
}

// this is a double fault, really
// why does it work? I have no idea.

static inline void start_system_timer()
{
	__asm__ __volatile__("int $0x08");
}

#endif /* _COMMON_H_*/
