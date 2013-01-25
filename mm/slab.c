#include <kernel/types.h>

/** right now: just a very simple implementation
 * slab allocators will be implemented later **/

extern uint32_t end;
uint32_t placement_addr = (uint32_t)&end;

#define ALIGN() \
	if ((placement_addr & 0xFFFFF000)) { \
		placement_addr &= 0xFFFFF000; \
		placement_addr += 0x1000; \
}

#define PLACE(siz) ({ \
	uint32_t tmp; \
	tmp = placement_addr; \
	placement_addr += siz; \
	tmp; \
})

/** generic kmalloc */
uint32_t kmalloc(size_t siz)
{
	uint32_t temp;
	temp = PLACE(siz);
	return temp;
}

/** page aligned */
uint32_t kmalloc_a(size_t siz)
{
	uint32_t temp;
	ALIGN();
	temp = PLACE(siz);
	return temp;
}

uint32_t kmalloc_p(size_t siz, uint32_t *phys)
{
	uint32_t temp;
	temp = PLACE(siz);
	*phys = placement_addr;
	return temp;
}

uint32_t kmalloc_ap(size_t siz, int align, uint32_t *phys)
{
	uint32_t temp;
	ALIGN();
	temp = PLACE(siz);
	*phys = placement_addr;
	return temp;
}
