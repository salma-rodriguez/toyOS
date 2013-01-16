#ifndef _COMMON_H_
#define _COMMON_H_

#ifndef _TYPES_H_
#define _TYPES_H_
#include <kernel/types.h>
#endif

#ifndef SEGMENT_FLAGS
#define SEGMENT_FLAGS

#define SEG_SAVL(X)      ((X) << 0x04) 
#define SEG_LONG(X)      ((X) << 0x05) 
#define SEG_SIZE(X)      ((X) << 0x06) 
#define SEG_GRAN(X)      ((X) << 0x07) 
#define SEG_PRES(X)      ((X) << 0x07) 
#define SEG_PRIV(X)     (((X) &  0x03) << 0x05)
#define SEG_DESCTYPE(X)  ((X) << 0x04)
	 
#define SEG_DATA_RD        0x00 
#define SEG_DATA_RDA       0x01 
#define SEG_DATA_RDWR      0x02 
#define SEG_DATA_RDWRA     0x03 
#define SEG_DATA_RDEXPD    0x04 
#define SEG_DATA_RDEXPDA   0x05 
#define SEG_DATA_RDWREXPD  0x06 
#define SEG_DATA_RDWREXPDA 0x07 
#define SEG_CODE_EX        0x08 
#define SEG_CODE_EXA       0x09 
#define SEG_CODE_EXRD      0x0A 
#define SEG_CODE_EXRDA     0x0B 
#define SEG_CODE_EXC       0x0C 
#define SEG_CODE_EXCA      0x0D 
#define SEG_CODE_EXRDC     0x0E 
#define SEG_CODE_EXRDCA    0x0F 
 
#define GDT_CODE_PL(X) SEG_PRES(1) 	| SEG_PRIV(X) | \
		   	SEG_DESCTYPE(1) | SEG_CODE_EXRD
		       
#define GDT_DATA_PL(X) SEG_PRES(1) 	| SEG_PRIV(X) | \
		   	SEG_DESCTYPE(1) | SEG_DATA_RDWR

#define GRANULARITY(L) SEG_GRAN(1) 	| SEG_SIZE(1) | \
			SEG_LONG(0)	| SEG_SAVL(0) | L

#define GDT_CODE_GATE(dpl, base, lim) 	({		\
	gdt_entry_t val;				\
	val = (gdt_entry_t) { 				\
	.limit  = lim & 0xFFFF,				\
	.base_l = base & 0xFFFF,			\
	.base_m = base >> 0x10 & 0xFF,			\
	.privge = GDT_CODE_PL(dpl),			\
	.granty = GRANULARITY(0xF),			\
	.base_h = base >> 0x18 & 0xFF };		\
	val;						\
})

#define GDT_DATA_GATE(dpl, base, lim) 	({		\
	gdt_entry_t val;				\
	val = (gdt_entry_t) { 				\
	.limit  = lim & 0xFFFF,				\
	.base_l = base & 0xFFFF,			\
	.base_m = base >> 0x10 & 0xFF,			\
	.privge = GDT_DATA_PL(dpl),			\
	.granty = GRANULARITY(0xF),			\
	.base_h = base >> 0x18 & 0xFF };		\
	val;						\
})

#define IDT_SET_GATE(base, sel, flags) ({		\
	idt_entry_t val;				\
	val = (idt_entry_t) {				\
	.base_lo = base & 0xFFFF,			\
	.base_hi = base >> 16 & 0xFFFF,			\
	.sel = sel,					\
	.always0 = 0,					\
	.flags = flags };				\
	val;						\
})

#endif /* SEGMENT_FLAGS */

__u8 inb(__u16 port);
__u16 inw(__u16 port);
void outb(__u16 port, __u8 value);

#endif /* _COMMON_H_*/
