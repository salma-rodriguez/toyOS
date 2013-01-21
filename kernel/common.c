#include <asm/common.h>
#include <kernel/types.h>

inline __u8 inportb(__u16 port)
{
	register __u8 ret;
	__asm__ __volatile__("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

inline __u16 inportw(__u16 port)
{
	register __u16 ret;
	__asm__ __volatile__("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

inline void outportb(__u16 port, __u8 value)
{
	__asm__ __volatile__("outb %1, %0" : : "dN" (port), "a" (value));
}
