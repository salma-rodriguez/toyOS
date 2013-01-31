#ifndef _DESC_DEFS_H_
#define _DESC_DEFS_H_

struct idt_entry_struct
{
	__u64	base_l : 0x10;
	__u64	select : 0x10;
	__u64	alzero : 0x08;
	__u64	eflags : 0x08;
	__u64	base_h : 0x10;
} __attribute__ ((packed));

struct idt_ptr_struct
{
	__u16	lim;
	__u32	base;		// address of first element
} __attribute__ ((packed));

struct gdt_entry_struct
{
	__u64	lim_lo : 0x10;
	__u64	base_l : 0x10;
	__u64	base_m : 0x08;
	__u64	access : 0x08;
	__u64	granty : 0x08;
	__u64	base_h : 0x08;
} __attribute__ ((packed));

struct gdt_ptr_struct
{
	__u16 	lim;		// upper 16 bits of selector limits
	__u32	base;		// address of the first gdt_entry_t
} __attribute__ ((packed));

typedef struct gdt_ptr_struct gdt_ptr_t;
typedef struct gdt_entry_struct gdt_entry_t;

typedef struct idt_ptr_struct idt_ptr_t;
typedef struct idt_entry_struct idt_entry_t;

#endif /* _DESC_DEFS_H_ */
