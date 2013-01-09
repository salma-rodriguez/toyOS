#include "multiboot.h"
#include <string.h>
#include <sprint.h>
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

	cs = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	printf("cs before memset: %s\n", cs);

	cs = memset(cs, 'H', 14);

	/* memset(&cs[0],'H',1);
	memset(&cs[1],'e',1);
	memset(&cs[2],'l',2);
	memset(&cs[4],'o',1);
	memset(&cs[5],',',1);
	memset(&cs[6],' ',1);
	memset(&cs[7],'w',1);
	memset(&cs[8],'o',1);
	memset(&cs[9],'r',1);
	memset(&cs[10],'l',1);
	memset(&cs[11],'d',1);
	memset(&cs[12],'!',1);
	memset(&cs[13],'\n',1); */

	printf("cs after memset: %s\n", cs);

	return 0;
}
