#include <string.h>
#include <asm/page.h>
#include <asm/common.h>
#include <kernel/isr.h>
#include <kernel/types.h>
#include <kernel/heap.h>
#include <kernel/panic.h>
#include <kernel/printk.h>

uint32_t *frames;
uint32_t nframes;

extern struct heap *kheap;
extern uint32_t placement_addr;

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

struct page_directory *kernel_directory;
struct page_directory *current_directory;

extern struct page_directory *clone_directory(struct page_directory *);

static void set_frame(uint32_t frame_addr)
{
	off_t off;
	uint32_t idx, frame;
	frame = frame_addr/PAGE_SIZ;
	idx = INDEX_FROM_BIT(frame);
	off = OFFSET_FROM_BIT(frame);
	frames[idx] |= (0x1 << off);
}

static void clear_frame(uint32_t frame_addr)
{
	uint32_t frame = frame_addr/PAGE_SIZ;
	uint32_t idx = INDEX_FROM_BIT(frame);
	uint32_t off = OFFSET_FROM_BIT(frame);
	frames[idx] &= ~(0x1 << off);
}

/* static uint32_t test_frame(uint32_t frame_addr)
{
	uint32_t frame = frame_addr/PAGE_SIZ;
	uint32_t idx = INDEX_FROM_BIT(frame);
	uint32_t off = OFFSET_FROM_BIT(frame);
	return (frames[idx] & (0x1  << off));
} */

static uint32_t first_frame()
{
	uint32_t i, j, to_test;
	for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
		if (frames[i] != 0xFFFFFFFF)
			for (j = 0; j < 32; j++) {
				to_test = 0x1 << j;
				if (!(frames[i] & to_test))
					return i * 4 * 8 + j;
			}
	return 0;
}

void alloc_frame(struct page *page, int kernel, int writable)
{
	uint32_t idx;
	if (page->frame) return;
	else
	{
		idx = first_frame();
		if (idx == (uint32_t)-1)
			PANIC("No free frames!");
		set_frame(idx*PAGE_SIZ);
		page->present = 1;
		page->rw = writable;
		page->user = kernel;
		page->frame = idx;
	}
}

void free_frame(struct page *page)
{
	uint32_t frame;
	if (!(frame = page->frame))
		return;
	else {
		clear_frame(frame);
		page->frame = 0x00000;
	}
}

void init_paging()
{
	size_t sz;
	uint32_t i, phys;
	uint32_t mem_end_page;

	printk("paging...\t\t");

	mem_end_page = 0x1000000;
	nframes = mem_end_page / PAGE_SIZ;

	sz = INDEX_FROM_BIT(nframes);
	frames = (uint32_t *)kmalloc(sz);
	memset(frames, 0, sz);

	kernel_directory = (struct page_directory *)
		kmalloc_a(sizeof(struct page_directory));
	memset(kernel_directory, 0, sizeof(struct page_directory));

	// don't do this...
	current_directory = kernel_directory;

	// do this instead...
	kernel_directory->physical_addr = (uint32_t)kernel_directory->tables_physical;

	for (i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += PAGE_SIZ)
		get_page(i, 1, kernel_directory);

	i = 0;
	while (i < placement_addr + PAGE_SIZ) {
		alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
		i += PAGE_SIZ;
	}

	for (i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += PAGE_SIZ)
		alloc_frame(get_page(i, 1, kernel_directory), 0, 0);

	// register_interrupt_handler(14, page_fault);

	switch_page_directory(kernel_directory);
	enable_paging();

	kheap = create_heap(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);

	current_directory = clone_directory(kernel_directory);
        switch_page_directory(current_directory);

	printk("done!\n");
}

struct page *get_page(uint32_t address, int creat, struct page_directory *dir)
{
	uint32_t temp;
	uint32_t table_idx;
	address /= PAGE_SIZ;
	table_idx = address / 1024;
	if (dir->tables[table_idx])
		return &dir->tables[table_idx]->pages[address%1024];
	else if(creat) {
		dir->tables[table_idx] = (struct page_table *)kmalloc_ap(sizeof(struct page_table), &temp);
		memset(dir->tables[table_idx], 0, PAGE_SIZ);
		dir->tables_physical[table_idx] = temp | 0x7;
		return &dir->tables[table_idx]->pages[address%1024];
	}
	return NULL;
}

void handle_page_fault(struct registers *regs)
{
	int rw;
	int id;
	int usr;
	int present;
	int reserved;
	uint32_t faulting_address;
	
	present = !(regs->err_code & PRESENT_MASK);
	rw = regs->err_code & RW_MASK;
	usr = regs->err_code & USER_MODE_MASK;
	reserved = regs->err_code & RSVD3_MASK;
	id = regs->err_code & ACCESSED_MASK;

	faulting_address = get_faulting_address();

	printk("page fault (%s %s %s %s %s) at 0x%lx\n",
		id? "instruction fetch":"",
		present? "present":"", rw? "read-write":"",
		usr? "user-mode":"", reserved? "reserved":"",
		faulting_address);
	PANIC("page fault!");
}
