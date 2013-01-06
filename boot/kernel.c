#include "multiboot.h"
#include <string.h>
#include <kernel/monitor.h>

int kmain(multiboot_info_t *mbd, unsigned int magic) {
	char *cs, *ct;
	monitor_clear();
	cs = "Hello, world!";

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		monitor_write("boot magic is incorrect");
		/* next: do something, like PANIC */
	}

	/* testing strlen */

	if (strlen(cs) == 13)
		monitor_write("Length is 13.\n");
	else
		monitor_write("Length is not 13.\n");

	/* testing strcmp */

	if (!strcmp(cs, "Hello, world!"))
		monitor_write("String is \'Hello, world!\'\n");
	else
		monitor_write("String is not \'Hello, world!\'\n");

	cs = "Hello, ";
	ct = "world!\n";
	
	/* testing strcat */

	monitor_write(strcat(cs,ct));

	/* testing strcpy */

	strcpy(cs, "Hello, world!\n");
	monitor_write(cs);

	return 0;
}
