#include <kernel/types.h>
#include <kernel/common.h>

__u8 inportb(__u16 port)
{
	register __u8 ret;
	asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

__u16 inportw(__u16 port)
{
	register __u16 ret;
	asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

void outportb(__u16 port, __u8 value)
{
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}
