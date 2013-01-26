#include <string.h>
#include <asm/common.h>
#include <asm/interrupt.h>
#include <kernel/types.h>
#include <kernel/tables.h>
#include <kernel/isr.h>
#include <kernel/printk.h>

#define CODE	0
#define DATA	1
#define DEBUG	1

#define NOINTS	256
#define MAXADDR 0xFFFFFFFF

#define ISR(isrnum) idt_set_gate(isrnum, (uint32_t)isr##isrnum)
#define IRQ(num, irqnum) idt_set_gate(num, (uint32_t)irq##irqnum)

static void gdt_init();
static void idt_init();

extern void gdt_flush(uint32_t);
extern void idt_flush(uint32_t);

static inline void idt_set_gate(uint8_t, uint32_t);
static inline void gdt_set_gate(uint8_t, uint8_t, uint8_t, 
		uint32_t, uint32_t);

extern isr_t interrupt_handlers[];

gdt_page_t  gdt_page;
idt_page_t  idt_page;

void init_descriptor_tables()
{
	DPRINTK("descriptor tables...\t");

	gdt_init();
	idt_init();

	memset(&interrupt_handlers, 0, sizeof(isr_t)*IDT_ENTRIES);

	DPRINTK("done!\n");
}

static void gdt_init()
{
	gdt_set_gate(0, 0, 0, 0, 0);
	gdt_set_gate(1, 0, CODE, 0, MAXADDR);
	gdt_set_gate(2, 0, DATA, 0, MAXADDR);
	gdt_set_gate(3, 3, CODE, 0, MAXADDR);
	gdt_set_gate(4, 3, DATA, 0, MAXADDR);

	gdt_page.gdt_ptr.base = (uint32_t)&gdt_page.gdt;
	gdt_page.gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
	
	gdt_flush((uint32_t)&gdt_page.gdt_ptr);
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
	idt_page.idt_ptr.base  = (uint32_t)&idt_page.idt;
	idt_page.idt_ptr.limit = sizeof(idt_entry_t) * IDT_ENTRIES - 1;

	memset(&idt_page.idt, 0, sizeof(idt_entry_t) * IDT_ENTRIES);

	remap_irq_table();
	
	ISR(0);
	ISR(1);
	ISR(2);
	ISR(3);
	ISR(4);
	ISR(5);
	ISR(6);
	ISR(7);
	ISR(8);
	ISR(9);
	ISR(10);
	ISR(11);
	ISR(12);
	ISR(13);
	ISR(14);
	ISR(15);
	ISR(16);
	ISR(17);
	ISR(18);
	ISR(19);
	ISR(20);
	ISR(21);
	ISR(22);
	ISR(23);
	ISR(24);
	ISR(25);
	ISR(26);
	ISR(27);
	ISR(28);
	ISR(29);
	ISR(30);
	ISR(31);
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

	idt_flush((uint32_t)&idt_page.idt_ptr);
}

static inline void gdt_set_gate(uint8_t num, uint8_t privilege, 
		uint8_t type, uint32_t base, uint32_t limit)
{
	gdt_page.gdt[num] = GDT_SET_GATE(privilege, type, base, limit);
}

static inline void idt_set_gate(uint8_t num, uint32_t base)
{
	idt_page.idt[num] = IDT_SET_GATE(base);
}
