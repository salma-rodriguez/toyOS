#ifndef _COMMON_H_
#define _COMMON_H_

__u8 inb(__u16 port);
__u16 inw(__u16 port);
void outb(__u16 port, __u8 value);

#endif
