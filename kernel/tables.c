#include <string.h>
#include <kernel/types.h>
#include <asm/segment.h>
#include <asm/interrupt.h>
#include <kernel/isr.h>
#include <kernel/common.h>

#define NOINTS	256
#define MAXADDR 0xFFFFFFFF

static void gdt_init();
static void idt_init();
extern void gdt_flush(uint32_t);
extern void idt_flush(uint32_t);
static inline void idt_set_gate(uint8_t, uint32_t);

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
	
	gdt_entries[0] = (gdt_entry_t) {0, 0, 0, 0, 0, 0};
	gdt_entries[1] = GDT_CODE_GATE(0, 0, MAXADDR);
	gdt_entries[2] = GDT_DATA_GATE(0, 0, MAXADDR);
	gdt_entries[3] = GDT_CODE_GATE(3, 0, MAXADDR);
	gdt_entries[4] = GDT_DATA_GATE(3, 0, MAXADDR);

	gdt_flush((uint32_t)&gdt_ptr);
}

static void remap_irq_table()
{
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x00);
	outb(0xA1, 0x00);
}

static void idt_init()
{
	idt_ptr.limit = sizeof(idt_entry_t) * NOINTS - 1;
	idt_ptr.base  = (uint32_t)&idt_entries;

	memset(&idt_entries, 0, sizeof(idt_entry_t) * NOINTS);

	// Remap irq table.
	remap_irq_table();
		
	idt_set_gate(0,  (uint32_t)isr0);
	idt_set_gate(1,  (uint32_t)isr1);
	idt_set_gate(2,  (uint32_t)isr2);
	idt_set_gate(3,  (uint32_t)isr3);
	idt_set_gate(4,  (uint32_t)isr4);
	idt_set_gate(5,  (uint32_t)isr5);
	idt_set_gate(6,  (uint32_t)isr6);
	idt_set_gate(7,  (uint32_t)isr7);
	idt_set_gate(8,  (uint32_t)isr8);
	idt_set_gate(9,  (uint32_t)isr9);
	idt_set_gate(10, (uint32_t)isr10);
	idt_set_gate(11, (uint32_t)isr11);
	idt_set_gate(12, (uint32_t)isr12);
	idt_set_gate(13, (uint32_t)isr13);
	idt_set_gate(14, (uint32_t)isr14);
	idt_set_gate(15, (uint32_t)isr15);
	idt_set_gate(16, (uint32_t)isr16);
	idt_set_gate(17, (uint32_t)isr17);
	idt_set_gate(18, (uint32_t)isr18);
	idt_set_gate(19, (uint32_t)isr19);
	idt_set_gate(20, (uint32_t)isr20);
	idt_set_gate(21, (uint32_t)isr21);
	idt_set_gate(22, (uint32_t)isr22);
	idt_set_gate(23, (uint32_t)isr23);
	idt_set_gate(24, (uint32_t)isr24);
	idt_set_gate(25, (uint32_t)isr25);
	idt_set_gate(26, (uint32_t)isr26);
	idt_set_gate(27, (uint32_t)isr27);
	idt_set_gate(28, (uint32_t)isr28);
	idt_set_gate(29, (uint32_t)isr29);
	idt_set_gate(30, (uint32_t)isr30);
	idt_set_gate(31, (uint32_t)isr31);
	idt_set_gate(32, (uint32_t)irq0);
	idt_set_gate(33, (uint32_t)irq1);
	idt_set_gate(34, (uint32_t)irq2);
	idt_set_gate(35, (uint32_t)irq3);
	idt_set_gate(36, (uint32_t)irq4);
	idt_set_gate(37, (uint32_t)irq5);
	idt_set_gate(38, (uint32_t)irq6);
	idt_set_gate(39, (uint32_t)irq7);
	idt_set_gate(40, (uint32_t)irq8);
	idt_set_gate(41, (uint32_t)irq9);
	idt_set_gate(42, (uint32_t)irq10);
	idt_set_gate(43, (uint32_t)irq11);
	idt_set_gate(44, (uint32_t)irq12);
	idt_set_gate(45, (uint32_t)irq13);
	idt_set_gate(46, (uint32_t)irq14);
	idt_set_gate(47, (uint32_t)irq15);

	idt_flush((uint32_t)&idt_ptr);
}

static inline void idt_set_gate(uint8_t num, uint32_t base)
{
	idt_entries[num] = IDT_SET_GATE(base);
}
