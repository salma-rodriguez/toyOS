#ifndef COMMON_H
#define COMMON_H

typedef int s32;
typedef char s8;
typedef short s16;
typedef unsigned int u32;
typedef unsigned char u8;
typedef unsigned short u16;

u8 inb(u16 port);
u16 inw(u16 port);
void outb(u16 port, u8 value);

#endif
