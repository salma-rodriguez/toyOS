#ifndef _KHEAP_H_
#define _KHEAP_H_

#define KHEAP_START		0xC0000000
#define KHEAP_INITIAL_SIZE	0x100000
#define HEAP_INDEX_SIZE		0x20000
#define HEAP_MAGIC		0x123890AB
#define HEAP_MIN_SIZE		0x70000

struct header
{
	uint32_t magic;
	uint8_t is_hole;
	uint32_t size;
}

struct footer
{
	uint32_t magic;
	struct header *header;
}

struct heap
{
	struct ordered_array *index;
	uint32_t start_address;
	uint32_t end_address;
	uint32_t max_address;
	uint8_t supervisor;
	uint8_t readonly;
}

uint32_t kmalloc(size_t siz);
uint32_t kmalloc_a(size_t siz);
uint32_t kmalloc_p(size_t siz, uint32_t *phys);
uint32_t kmalloc_ap(size_t siz, uint32_t *phys);

struct heap *create_heap(uint32_t, uint32_t, uint32_t, uint8_t, uint8_t);
void *alloc(uint32_t size, uint8_t page_align, struct heap *heap);
void free(void *p, struct heap *heap);

#endif /* _KHEAP_H_ */
