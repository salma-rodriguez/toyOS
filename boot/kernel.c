#include "multiboot.h"
#include <kernel/monitor.h>

int kmain(struct multiboot_info *mbd, unsigned int magic) {
	monitor_clear();
	monitor_write("Hello, world!");
	return 0;
}
