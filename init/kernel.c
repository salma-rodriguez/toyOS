#include "multiboot.h"
#include <stdio.h>
#include <string.h>
#include <kernel/monitor.h>

int kmain(multiboot_info_t *mbd, unsigned int magic) {
	char *cs, *ct;
	monitor_clear();

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		printf("boot magic is incorrect");
		/* next: do something, like PANIC */
	}

	cs = "Hello, world!";

	/* testing strcmp */

	if (!strcmp(cs, "Hello, world!"))
		printf("String is \'Hello, world!\'\n");
	else
		printf("String is not \'Hello, world!\'\n");

	cs = "Hello, ";
	ct = "world!\n"; 
	
	/* testing strcat */

	printf(strcat(cs,ct));

	/* testing strcpy */

	strcpy(cs, "Hello, world!\n");
	printf(cs);

	/* testing strlen & printf */

	cs = "Hello, world!\n";
	printf("The length of the string is: %d\n", strlen(cs));

	/* testing memset */

	cs = 0x0000000000000000000000000000;
	printf("cs before memset: %s\n", cs);

	cs = memset(cs, 'H', 14);
	printf("cs after memset: %s\n", cs);

	/* testing interrupts */

	// __asm__ __volatile__("int $0x3");
	// __asm__ __volatile__("int $0x4");

	return 0;
}
