#include <asm/page.h>
#include <kernel/heap.h>
#include <kernel/types.h>

struct heap *kheap = NULL;

extern uint32_t end;
uint32_t placement_addr = (uint32_t)&end;
extern struct page_directory *kernel_directory;

extern void free_frame(struct page *);
extern void alloc_frame(struct page *, int, int);
static uint32_t kmalloc_int(size_t, int, uint32_t *);

#define ALIGN() \
	if (placement_addr & 0xFFFFF000) { \
		placement_addr &= 0xFFFFF000; \
		placement_addr += PAGE_SIZ; \
	}

#define PLACE(siz) ({ \
	uint32_t tmp; \
	tmp = placement_addr; \
	placement_addr += siz; \
	tmp; \
})

void kfree(void *p)
{
	free(p, kheap);
}

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
        void *addr;
	uint32_t temp;

	if (kheap)
	{
                addr = alloc(siz, (uint8_t)align, kheap);
                if (phys)
                {
                        struct page *page;
                        page = get_page((uint32_t)addr, 0, kernel_directory);
                        *phys = page->frame*PAGE_SIZ + ((uint32_t)addr&0xFFF);
                }
                return (uint32_t)addr;
        }

        if (align) ALIGN();               
        if (phys) *phys = placement_addr;
        temp = PLACE(siz);

	return temp;
}

static int compare(any_t a, any_t b)
{
	return (((struct header *)a)->size < ((struct header *)b)->size)? 1:0;
}

struct heap *create_heap(uint32_t start, uint32_t end_addr, uint32_t max, uint8_t supervisor, uint8_t readonly)
{
	struct heap *heap;
	struct header *hole;

	heap = (struct heap *)kmalloc(sizeof(struct heap));

	// ASSERT(start%PAGE_SIZ == 0);
	// ASSERT(end_addr%PAGE_SIZ == 0);
	
	heap->index = place_ordered_array((void *)start, HEAP_INDEX_SIZE, compare);

	start += HEAP_INDEX_SIZE * sizeof(any_t);

	if (start & 0xFFFFF000) {
		start &= 0xFFFFF000;
		start += PAGE_SIZ;
	}

	heap->start_address = start;
	heap->end_address = end_addr;
	heap->max_address = max;
	heap->supervisor = supervisor;
	heap->readonly = readonly;

	hole = (struct header *)start;
	hole->size = end_addr - start;
	hole->magic = HEAP_MAGIC;
	hole->is_hole = 1;
	heap->index.insert((void *)hole, &heap->index);

	return heap;
}

static void expand(size_t new_size, struct heap *heap)
{
	uint32_t i;
	size_t old_size;

	// ASSERT(new_size > heap->end_address - heap->start_address);
	
	if (new_size & 0xFFFFF000) {
		new_size &= 0xFFFFF000;
		new_size += PAGE_SIZ;
	}

	// ASSERT(heap->start_address+new_size <= heap->max_address);
	
	old_size = heap->end_address - heap->start_address;
	i = old_size;
	while (i < new_size) {
		alloc_frame(get_page(heap->start_address+i, 1, kernel_directory),
			(heap->supervisor)?1:0, (heap->readonly)?0:1);
		i += PAGE_SIZ;
	}
	heap->end_address = heap->start_address+new_size;
}

static size_t contract(size_t new_size, struct heap *heap)
{
	size_t i;
	size_t old_size;

	// ASSERT(new_size < heap->end_address - heap->start_address);
	
	if (new_size & PAGE_SIZ) {
		new_size &= PAGE_SIZ;
		new_size += PAGE_SIZ;
	}
	if (new_size < HEAP_MIN_SIZE)
		new_size = HEAP_MIN_SIZE;
	old_size = heap->end_address - heap->start_address;
	i = old_size - PAGE_SIZ;
	while (new_size < i) {
		free_frame(get_page(heap->start_address+i, 0, kernel_directory));
		i -= PAGE_SIZ;
	}
	heap->end_address = heap->start_address + new_size;

	return new_size;
}

static int32_t find_smallest_hole(size_t size, uint8_t page_align, struct heap *heap)
{
	int i;
	soff_t offset;
	ssize_t hole_size;
	uint32_t location;
	struct header *header;

	i=0;
	while (i < heap->index.count) {
		header = (struct header *)heap->index.lookup(i, &heap->index);
		if (page_align) {
			location = (uint32_t)header;
			offset = 0;
			if ((location + sizeof(struct header)) & 0xFFFFF000)
				offset = PAGE_SIZ - (location+sizeof(struct header))%PAGE_SIZ;
			hole_size = (ssize_t)header->size - offset;
			if (hole_size >= (ssize_t)size)
				break;
		} else if (header->size >= size)
			break;
		i++;
	}

	if (i == heap->index.count)
		return -1;

	return i;
}

void *alloc(size_t size, uint8_t page_align, struct heap *heap)
{
	size_t new_size;
	uint32_t iterator;
	uint32_t old_length;
	uint32_t new_length;
	size_t orig_hole_size;
	struct header *header;
	struct footer *footer;
	uint32_t new_location;
	uint32_t orig_hole_pos;
	uint32_t old_end_address;
	uint32_t idx, value, tmp;
	struct header *hole_header;
	struct footer *hole_footer;
	struct header *block_header;
	struct footer *block_footer;
	struct header *orig_hole_header;

	new_size = size + sizeof(struct header) + sizeof(struct footer);
	iterator = find_smallest_hole(new_size, page_align, heap);
	
	if (iterator == -1) {
		old_length = heap->end_address - heap->start_address;
		old_end_address = heap->end_address;
		expand(old_length + new_size, heap);
		new_length = heap->end_address - heap->start_address;

		iterator = 0;

		idx = -1; value = 0x0;
		while (iterator < heap->index.count)
		{
			tmp = (uint32_t)heap->index.lookup(iterator, &heap->index);
			if (tmp > value) {
				value = tmp;
				idx = iterator;
			}
			iterator++;
		}

		if (idx == -1) {
			header = (struct header *)old_end_address;
			header->magic = HEAP_MAGIC;
			header->size = new_length - old_length;
			header->is_hole = 1;
			footer = (struct footer *)(old_end_address + header->size - sizeof(struct footer));
			footer->magic = HEAP_MAGIC;
			footer->header = header;
			heap->index.insert((void *)header, &heap->index);
		} else {
			header = heap->index.lookup(idx, &heap->index);
			header->size += new_length - old_length;
			footer = (struct footer *)((uint32_t)header + header->size - sizeof(struct footer));
			footer->header = header;
			footer->magic = HEAP_MAGIC;
		}

		return alloc(size, page_align, heap);
	}

	orig_hole_header = (struct header *)heap->index.lookup(iterator, &heap->index);
	orig_hole_pos = (uint32_t)orig_hole_header;
	orig_hole_size = orig_hole_header->size;

	if (orig_hole_size - new_size < sizeof(struct header) + sizeof(struct footer)) {
		size += orig_hole_size - new_size;
		new_size = orig_hole_size;
	}
	
	if (page_align && orig_hole_pos & 0xFFFFF000) {
		new_location = orig_hole_pos + PAGE_SIZ - (orig_hole_pos & 0xFFF) - sizeof(struct header);
		hole_header = (struct header *)orig_hole_pos;
		hole_header->size = PAGE_SIZ - (orig_hole_pos & 0xFFF) - sizeof(struct header);
		hole_header->magic = HEAP_MAGIC;
		hole_header->is_hole = 1;
		hole_footer = (struct footer *)((uint32_t)new_location - sizeof(struct footer));
		hole_footer->magic = HEAP_MAGIC;
		hole_footer->header = hole_header;
		orig_hole_pos = new_location;
		orig_hole_size = orig_hole_size - hole_header->size;
	} else heap->index.remove(iterator, &heap->index);

	block_header = (struct header *)orig_hole_pos;
	block_header->magic = HEAP_MAGIC;	
	block_header->is_hole = 0;
	block_header->size = new_size;
	block_footer = (struct footer *)(orig_hole_pos + sizeof(struct header) + size);
	block_footer->magic = HEAP_MAGIC;
	block_footer->header = block_header;

	if (orig_hole_size - new_size > 0) {
		hole_header = (struct header *)(orig_hole_pos + sizeof(struct header) + size + sizeof(struct footer));
		hole_header->magic = HEAP_MAGIC;
		hole_header->is_hole = 1;
		hole_header->size = orig_hole_size - new_size;
		hole_footer = (struct footer *)((uint32_t)hole_header + orig_hole_size - new_size - sizeof(struct footer));
		if ((uint32_t)hole_footer < heap->end_address) {
			hole_footer->magic = HEAP_MAGIC;
			hole_footer->header = hole_header;
		}
		heap->index.insert((void *)hole_header, &heap->index);
		return (void *)((uint32_t)block_header + sizeof(struct header));
	}

	return NULL;
}

void free(void *p, struct heap *heap)
{
	char do_add;
	uint32_t iterator;
	uint32_t cache_size;
	uint32_t old_length;
	uint32_t new_length;
	struct header *header;
	struct footer *footer;
	struct header *test_header;
	struct footer *test_footer;

	if (!p) return;

	header = (struct header *)((uint32_t)p - sizeof(struct header));
	footer = (struct footer *)((uint32_t)header + header->size - sizeof(struct footer));

	// ASSERT(header->magic == HEAP_MAGIC);
	// ASSERT(footer->magic == HEAP_MAGIC);
	
	header->is_hole = 1;
	do_add = 1;

	test_footer = (struct footer *)((uint32_t)header - sizeof(struct footer));
	if (test_footer->magic == HEAP_MAGIC &&
		test_footer->header->is_hole)
	{
		cache_size = header->size;
		header = test_footer->header;
		footer->header = header;
		header->size += cache_size;
		do_add = 0;
	}

	test_header = (struct header *)((uint32_t)footer + sizeof(struct footer));
	if (test_header->magic == HEAP_MAGIC && test_header->is_hole)
	{
		header->size += test_header->size;
		test_footer = (struct footer *)((uint32_t)test_header + test_header->size - sizeof(struct footer));
		footer = test_footer;
		iterator = 0;
		while ((iterator < heap->index.count) &&
			(heap->index.lookup(iterator, &heap->index) != (void *)test_header))
			iterator++;
		// ASSERT(iterator < heap->index.count);
		heap->index.remove(iterator, &heap->index);
	}

	if ((uint32_t)footer + sizeof(struct footer) == heap->end_address)
	{
		old_length = heap->end_address - heap->start_address;
		new_length = contract((uint32_t)header - heap->start_address, heap);

		if (header->size - (old_length - new_length) > 0) {
			header->size -= old_length - new_length;
			footer = (struct footer *)((uint32_t)header + header->size - sizeof(struct footer));
			footer->magic = HEAP_MAGIC;
			footer->header = header;
		} else {
			iterator = 0;
			while ((iterator < heap->index.size) &&
				(heap->index.lookup(iterator, &heap->index) != (void *)test_header))
				iterator++;
			if (iterator < heap->index.size)
				heap->index.remove(iterator, &heap->index);
		}
		if (do_add == 1)
			heap->index.insert((void *)header, &heap->index);
	}
}
