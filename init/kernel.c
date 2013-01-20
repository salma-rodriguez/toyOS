#include "multiboot.h"
#include <stdio.h>
#include <string.h>
#include <kernel/types.h>
#include <kernel/timer.h>
#include <kernel/monitor.h>

extern void init_descriptor_tables();

int kmain(multiboot_info_t *mbd, unsigned int magic) {

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		printf("boot magic is incorrect");
		/* next: do something, like PANIC */
	}

	monitor_clear();

	/* initialize discriptor tables */

	init_descriptor_tables();
			
	init_timer(50);

	// __asm__ __volatile__("int $0x3");
	// __asm__ __volatile__("int $0x4");
	
	__asm__ __volatile__("sti");
	__asm__ __volatile__("int $0x8");
			
	return 0;
}
