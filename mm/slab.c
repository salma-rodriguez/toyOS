#include <kernel/types.h>

/** right now: just a very simple implementation
 * slab allocators will be implemented later **/

extern uint32_t end;
uint32_t placement_addr = (uint32_t)&end;

static uint32_t kmalloc_int(size_t, int, uint32_t *);

#define ALIGN() \
	if (placement_addr & 0x00000FFF) { \
		placement_addr &= 0xFFFFF000; \
		placement_addr += 0x1000; \
	}

#define PLACE(siz) ({ \
	uint32_t tmp; \
	tmp = placement_addr; \
	placement_addr += siz; \
	tmp; \
})

uint32_t kmalloc(size_t siz)
{
	return kmalloc_int(siz, 0, NULL);
}

uint32_t kmalloc_a(size_t siz)
{
	return kmalloc_int(siz, 1, NULL);
}

uint32_t kmalloc_p(size_t siz, uint32_t *phys)
{
	return kmalloc_int(siz, 0, phys);
}

uint32_t kmalloc_ap(size_t siz, uint32_t *phys)
{
	return kmalloc_int(siz, 1, phys);
}

static uint32_t kmalloc_int(size_t siz, int align, uint32_t *phys)
{
	uint32_t temp;
	if (align) ALIGN();
	if (phys) *phys = placement_addr;
	temp = PLACE(siz);
	return temp;
}
