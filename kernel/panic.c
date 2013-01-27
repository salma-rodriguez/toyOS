#include <stdarg.h>
#include <kernel/panic.h>
#include <kernel/printk.h>
#include <asm/common.h>

void panic(const char * fmt, ...)
{
	static char buf[1024];
	va_list args;

	disable_interrupts();

	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	printk("kernel panic - not syncing: %s\n", buf);

	while(1);
}
