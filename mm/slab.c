#include <kernel/types.h>

/** right now: just a very simple implementation
 * slab allocators will be implemented later **/

uint32_t kmalloc(size_t siz, int align, uint32_t *phys)
{
	uint32_t temp;
	if (align == 1 && (placement_addr & 0xFFFFF000)) {
		placement_addr &= 0xFFFFF000;
		placement_addr += 0x1000;
	}
	if (phys)
		*phys = placement_addr;
	temp = placement_addr;
	placement_addr += siz;
	return temp;
}
