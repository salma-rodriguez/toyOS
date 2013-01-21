#ifndef _GDT_H_
#define _GDT_H_

#define PAGE_SIZE 1 << 12

#include <asm/desc.h>

struct gdt_page
{
	gdt_ptr_t gdt_ptr;
	gdt_entry_t gdt[GDT_ENTRIES];
} __attribute__ ((aligned(PAGE_SIZE)));

struct idt_page
{
	idt_ptr_t idt_ptr;
	idt_entry_t idt[IDT_ENTRIES];
} __attribute__ ((aligned(PAGE_SIZE)));

typedef struct gdt_page gdt_page_t;
typedef struct idt_page idt_page_t;

#endif /* _GDT_H_ */
