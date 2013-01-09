#include <kernel/types.h>
#include <kernel/common.h>

__u8 inb(__u16 port)
{
	__u8 ret;
	asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

__u16 inw(__u16 port)
{
	__u16 ret;
	asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

void outb(__u16 port, __u8 value)
{
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

