#include "multiboot.h"
#include <asm/page.h>
#include <asm/common.h>
#include <kernel/isr.h>
#include <kernel/types.h>
#include <kernel/timer.h>
#include <kernel/panic.h>
#include <kernel/monitor.h>
#include <kernel/printk.h>
#include <kernel/handler.h>
#include <kernel/tables.h>
#include <kernel/heap.h>

extern struct heap *kheap;

int kmain(multiboot_info_t *mbd, uint32_t magic) {

	uint32_t a, b, c, d;

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		PANIC("boot pagic is incorrect\n");
	}

	monitor_clear();

	printk("initializing...\n");

	disable_interrupts();

	init_descriptor_tables();
	init_irq_handlers();
	init_fault_handlers();
	init_timer(PIT_FREQUENCY);

	a = kmalloc(8);

	init_paging();

	enable_interrupts();

	b = kmalloc(8);
	c = kmalloc(8);

	printk("a: %lx, b: %lx\nc: %lx", a, b, c);

	kfree((void *)c);
	kfree((void *)b);

	d = kmalloc(12);

	printk(", d: %lx\n", d);

	// testing division by zero
	
	// int i;
	// i = 500 / 0;
	
	// testing page fault
	
	/* uint32_t *ptr = (uint32_t *)0xA0000000;
	uint32_t do_page_fault = *ptr; */

	while(1);

	return 0;
}
