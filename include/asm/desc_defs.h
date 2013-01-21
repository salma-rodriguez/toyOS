#ifndef _DESC_DEFS_H_
#define _DESC_DEFS_H_

struct idt_entry_struct
{
	__u16 	base_l;		// low 16 bits of jump address
	__u16	sel;
	__u8	zero;
	__u8	flags;		// more flags
	__u16	base_h;		// upper 16 bits of jump address
} __attribute__ ((packed));

struct idt_ptr_struct
{
	__u16	limit;
	__u32	base;		// address of first element
} __attribute__ ((packed));

struct gdt_entry_struct
{
	__u16	 lim_lo;	// lower 16 bits of limit
	__u16	 base_l;	// lower 16 bits of the base
	__u8	 base_m;	// middle eight bits of the base
	__u8	 privge;	// access flags, determines ring
	__u8	 granty;
	__u8	 base_h;	// last 8 bits of the base
} __attribute__ ((packed));

struct gdt_ptr_struct
{
	__u16 	limit;		// upper 16 bits of selector limits
	__u32	base;		// address of the first gdt_entry_t
} __attribute__ ((packed));

typedef struct gdt_ptr_struct gdt_ptr_t;
typedef struct gdt_entry_struct gdt_entry_t;

typedef struct idt_ptr_struct idt_ptr_t;
typedef struct idt_entry_struct idt_entry_t;

#endif /* _DESC_DEFS_H_ */
