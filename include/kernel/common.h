#ifndef COMMON_H
#define COMMON_H

__u8 inb(__u16 port);
__u16 inw(__u16 port);
void outb(__u16 port, __u8 value);

#endif
