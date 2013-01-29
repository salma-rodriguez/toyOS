#ifndef _GDT_H_
#define _GDT_H_

#include <asm/desc.h>

#define PAGE_SIZE 	(1 << 12)

#define GDT_ENTRIES 	5
#define IDT_ENTRIES 	256

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

void init_descriptor_tables();

typedef struct gdt_page gdt_page_t;
typedef struct idt_page idt_page_t;

#endif /* _GDT_H_ */
