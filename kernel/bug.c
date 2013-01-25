#include <stdio.h>
#include <kernel/bug.h>
#include <kernel/types.h>

void bug(const char *message, const char *file, uint32_t line_no)
{
	printf("BUG: %s at %s : line %d.\n", message, file, line_no);
}
