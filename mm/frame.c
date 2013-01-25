#include <stdio.h>
#include <string.h>
#include <kernel/isr.h>
#include <kernel/types.h>
#include <kernel/kheap.h>
#include <kernel/paging.h>
#include <kernel/panic.h>

uint32_t *frames;
uint32_t nframes;

extern uint32_t placement_addr;

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

// struct page_directory *kernel_directory;
struct page_directory *current_directory;

static void set_frame(uint32_t frame_addr)
{
	uint32_t frame = frame_addr/0x1000;
	uint32_t idx = INDEX_FROM_BIT(frame);
	uint32_t off = OFFSET_FROM_BIT(frame);
	frames[idx] |= (0x1 << off);
}

static void clear_frame(uint32_t frame_addr)
{
	uint32_t frame = frame_addr/0x1000;
	uint32_t idx = INDEX_FROM_BIT(frame);
	uint32_t off = OFFSET_FROM_BIT(frame);
	frames[idx] &= ~(0x1 << off);
}

static uint32_t test_frame(uint32_t frame_addr)
{
	uint32_t frame = frame_addr/0x1000;
	uint32_t idx = INDEX_FROM_BIT(frame);
	uint32_t off = OFFSET_FROM_BIT(frame);
	return (frames[idx] & (0x1  << off));
}

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
		set_frame(idx*0x1000);
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
		page->frame = 0x0;
	}
}

void init_paging()
{
	uint32_t i;
	uint32_t mem_end_page;
	struct page_directory *kernel_directory;

	mem_end_page = 0x1000000;
	nframes = mem_end_page / 0x1000;
	frames = (uint32_t *)kmalloc(INDEX_FROM_BIT(nframes));
	memset(frames, 0, INDEX_FROM_BIT(nframes));

	kernel_directory = (struct page_directory *)kmalloc_a(sizeof(struct page_directory));
	memset(kernel_directory, 0, sizeof(struct page_directory));
	current_directory = kernel_directory;

	i = 0;
	while (i < placement_addr) {
		alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
		i += 0x1000;
	}

	register_interrupt_handler(14, (isr_t)page_fault);
	switch_page_directory(kernel_directory);
}

void switch_page_directory(struct page_directory *dir)
{
	uint32_t cr0;
	current_directory = dir;
	asm volatile("mov %0, %%cr3":: "r" (&dir->tables_physical));
	// asm volatile("mov %%cr0, %0": "=r" (cr0));
	// cr0 |= 0x80000000;
	// asm volatile("mov %0, %%cr0":: "r" (cr0));
}

struct page *get_page(uint32_t address, int creat, struct page_directory *dir)
{
	uint32_t temp;
	uint32_t table_idx;
	address /= 0x1000;
	table_idx = address / 1024;
	if (dir->tables[table_idx])
		return &dir->tables[table_idx]->pages[address%1024];
	else if(creat) {
		dir->tables[table_idx] = (struct page_table *)kmalloc_ap(sizeof(struct page_table), &temp);
		memset(dir->tables[table_idx], 0, 0x1000);
		dir->tables_physical[table_idx] = temp | 0x7;
		return &dir->tables[table_idx]->pages[address%1024];
	} else
		return 0;
}

void page_fault(struct registers regs)
{
	int rw;
	int id;
	int usr;
	int present;
	int reserved;
	uint32_t faulting_address;

	asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
	
	present = !(regs.err_code & 0x1);
	rw = regs.err_code & 0x2;
	usr = regs.err_code & 0x4;
	reserved = regs.err_code & 0x8;
	id = regs.err_code & 0x10;

	printf("Page fault! ( %s %s %s %s ) at 0x%lx\n",
		id? "instruction fetch":"",
		present? "present":"", rw? "read-only":"",
		usr? "user-mode":"", reserved? "reserved":"");
	PANIC("page fault");
}
