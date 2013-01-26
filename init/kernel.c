#include <stdio.h>
#include "multiboot.h"
#include <asm/page.h>
#include <asm/common.h>
#include <kernel/types.h>
#include <kernel/timer.h>
#include <kernel/panic.h>
#include <kernel/monitor.h>

extern void init_descriptor_tables();

int kmain(multiboot_info_t *mbd, uint32_t magic) {

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		PANIC("boot pagic is incorrect\n");
	}

	monitor_clear();

	disable_interrupts();

	init_descriptor_tables();
	init_timer(PIT_FREQUENCY);
	// init_paging();

	enable_interrupts();
	
	// uint32_t *ptr = (uint32_t *)0xA0000000;
	// uint32_t do_page_fault = *ptr;

	while(1);

	return 0;
}
