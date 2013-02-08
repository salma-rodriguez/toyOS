#include <kernel/types.h>

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

static int32_t find_smallest_hole(size_t size, uint8_t page_align, struct heap *heap)
{
	int i;
	off_t offset;
	ssize_t hole_size;
	uint32_t location;
	struct header *header

	i=0;
	while (i < heap->index->size) {
		header = (struct header *)lookup_ordered_array(i, heap->index);
		if (page_align > 0) {
			location = (uint32_t)header;
			offset = 0;
			if ((location+sizeof(struct header)) & 0xFFFFF000 != 0)
				offset = PAGE_SIZE - (location+sizeof(struct header))%PAGE_SIZE;
			hole_size = (ssize_t)header->size - offset;
			if (hole_size >= (ssize_t)size)
				break;
		} else if (header->size >= size)
			break;
		i++;
	}

	if (i == heap->index->size)
		return -1;

	return i;

}
