#include <string.h>
#include <kernel/types.h>
#include <asm/segment.h>
#include <asm/interrupt.h>
#include <kernel/isr.h>
#include <kernel/common.h>

#define CODE	0
#define DATA	1

#define NOINTS	256
#define MAXADDR 0xFFFFFFFF

#define IDT(isrnum) idt_set_gate(isrnum, (uint32_t)isr##isrnum)
#define IRQ(num, irqnum) idt_set_gate(num, (uint32_t)irq##irqnum)

static void gdt_init();
static void idt_init();

extern void gdt_flush(uint32_t);
extern void idt_flush(uint32_t);

static inline void idt_set_gate(uint8_t, uint32_t);
static inline void gdt_set_gate(uint8_t, uint8_t, uint8_t, 
		uint32_t, uint32_t);

extern isr_t interrupt_handlers[];

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;
idt_entry_t idt_entries[NOINTS];
idt_ptr_t   idt_ptr;

void init_descriptor_tables()
{
	gdt_init();
	idt_init();

	memset(&interrupt_handlers, 0, sizeof(isr_t)*NOINTS);
}

static void gdt_init()
{
	gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
	gdt_ptr.base = (uint32_t)&gdt_entries;
	
	gdt_set_gate(0, 0, 0, 0, 0);
	gdt_set_gate(1, 0, CODE, 0, MAXADDR);
	gdt_set_gate(2, 0, DATA, 0, MAXADDR);
	gdt_set_gate(3, 3, CODE, 0, MAXADDR);
	gdt_set_gate(4, 3, DATA, 0, MAXADDR);
	
	gdt_flush((uint32_t)&gdt_ptr);
}

static void remap_irq_table()
{
	outportb(0x20, 0x11);
	outportb(0xA0, 0x11);
	outportb(0x21, 0x20);
	outportb(0xA1, 0x28);
	outportb(0x21, 0x04);
	outportb(0xA1, 0x02);
	outportb(0x21, 0x01);
	outportb(0xA1, 0x01);
	outportb(0x21, 0x00);
	outportb(0xA1, 0x00);
}

static void idt_init()
{
	int i, j;
	idt_ptr.limit = sizeof(idt_entry_t) * NOINTS - 1;
	idt_ptr.base  = (uint32_t)&idt_entries;

	memset(&idt_entries, 0, sizeof(idt_entry_t) * NOINTS);

	// Remap irq table
	
	remap_irq_table();
		
	// for (i = 0; i < 32; ++i)
	
	IDT(1);
	IDT(2);
	IDT(3);
	IDT(4);
	IDT(5);
	IDT(6);
	IDT(7);
	IDT(8);
	IDT(9);
	IDT(10);
	IDT(11);
	IDT(12);
	IDT(13);
	IDT(14);
	IDT(15);
	IDT(16);
	IDT(17);
	IDT(18);
	IDT(19);
	IDT(20);
	IDT(21);
	IDT(22);
	IDT(23);
	IDT(24);
	IDT(25);
	IDT(26);
	IDT(27);
	IDT(28);
	IDT(29);
	IDT(30);
	IDT(31);

	// for (j = 0; j < 16; ++i, ++j) IRQ(i, j);
	
	IRQ(32, 0);
	IRQ(33, 1);
	IRQ(34, 2);
	IRQ(35, 3);
	IRQ(36, 4);
	IRQ(37, 5);
	IRQ(38, 6);
	IRQ(39, 7);
	IRQ(40, 8);
	IRQ(41, 9);
	IRQ(42, 10);
	IRQ(43, 11);
	IRQ(44, 12);
	IRQ(45, 13);
	IRQ(46, 14);
	IRQ(47, 15);

	idt_flush((uint32_t)&idt_ptr);
}

static inline void gdt_set_gate(uint8_t num, uint8_t privilege, 
		uint8_t type, uint32_t base, uint32_t limit)
{
	gdt_entries[num] = GDT_SET_GATE(privilege, type, base, limit);
}

static inline void idt_set_gate(uint8_t num, uint32_t base)
{
	idt_entries[num] = IDT_SET_GATE(base);
}
