#ifndef _COMMON_H_
#define _COMMON_H_

#include <kernel/types.h>

#define EOI 0x20

#define PICS 0xA0
#define PICM 0x20

extern __u8 inportb(__u16 port);
extern __u16 inportw(__u16 port);
extern void outportb(__u16 port, __u8 value);

static inline void send_eoi_slave()
{
	outportb(PICS, EOI);
}

static inline void send_eoi_master()
{
	outportb(PICM, EOI);
}

static inline void enable_interrupts()
{
	__asm__ __volatile__ ("sti");
}

static inline void disable_interrupts()
{
	__asm__ __volatile__ ("cli");
}                                                        

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) & ((TYPE *)0)->MEMBER)
#endif

#ifndef container_of
#define container_of(ptr, type, member) ({                      \
        const typeof(((type *)0)->member) * __mptr = (ptr);     \
        (type *)((char *)__mptr - offsetof(type, member)); })
#endif

#endif /* _COMMON_H_*/
