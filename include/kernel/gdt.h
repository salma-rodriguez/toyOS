#ifndef _GDT_H_
#define _GDT_H_

#include <asm/desc_defs.h>

struct gdt_page
{
	gdt_entry_t gdt[NO_ENTRIES];
}

#endif /* _GDT_H_ */
