#include <stdio.h>
#include "multiboot.h"
#include <asm/common.h>
#include <kernel/types.h>
#include <kernel/timer.h>
#include <kernel/monitor.h>

extern void init_descriptor_tables();

int kmain(multiboot_info_t *mbd, uint32_t magic) {

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		printf("boot magic is incorrect");
		/* next: do something, like PANIC */
	}

	monitor_clear();

	/* initialize discriptor tables */

	init_descriptor_tables();

	// __asm__ __volatile__("int $0x03");
	// __asm__ __volatile__("int $0x04");

	init_timer(50);

	return 0;
}
