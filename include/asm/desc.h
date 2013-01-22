#ifndef _DESC_H_
#define _DESC_H_

#include <asm/segment.h>
#include <asm/desc_defs.h>

#define GDT_ENTRIES 5
#define IDT_ENTRIES 256

// This structure contains the value of one GDT entry.
// We use the attribute 'pacdked' to tell GCC not to change
// any of the aligment in the structure

void init_descriptor_tables();

#define SET_GDT_BASE(base) 					\
	.base_l = base & 0xFFFF, 				\
	.base_m = (base >> 0x10) & 0xFF, 			\
	.base_h = (base >> 0x18) & 0xFF

#define SET_GDT_LOW(lim) .lim_lo = lim & 0xFFFF

#define SET_GDT_DPL(seg, dpl) \
	.privge = seg? GDT_DATA_PL(dpl): GDT_CODE_PL(dpl)

#define SET_GDT_GRAN(lim, len) \
	.granty = ((lim & 0xFFFF) | ((GRANULARITY(len)) & 0xF0))

#define SET_IDT_BASE(base)					\
	.base_l = base & 0xFFFF,				\
	.base_h = (base >> 16) & 0xFFFF

#define SET_IDT_ZERO() .zero = 0x00
#define SET_IDT_SELECTOR(sl) .sel = sl
#define SET_IDT_FLAGS(flag) .flags = flag

// static inline void set_desc_base(__u64 base);

#define GDT_SET_GATE(dpl, seg, base, lim) ({			\
	gdt_entry_t val;					\
	val = (gdt_entry_t) { 					\
		SET_GDT_LOW(lim),				\
		SET_GDT_BASE(base),				\
		SET_GDT_DPL(seg, dpl),				\
		SET_GDT_GRAN(lim, 0xF)				\
	}; val;							\
})

#define IDT_SET_GATE(base) ({					\
	idt_entry_t val;					\
	val = (idt_entry_t) {					\
		SET_IDT_ZERO(),					\
		SET_IDT_BASE(base),				\
		SET_IDT_FLAGS(0x8E),				\
		SET_IDT_SELECTOR(0x08)				\
	}; val;							\
})

#endif /* _DESC_H_ */
