#ifndef _PAGING_H_
#define _PAGING_H_

#include <kernel/isr.h>
#include <kernel/common.h>

struct page
{
	uint32_t present	: 1;
	uint32_t rw		: 1;
	uint32_t user		: 1;
	uint32_t accessed	: 1;
	uint32_t dirty		: 1;
	uint32_t unused		: 7;
	uint32_t frame		: 20;
}

struct page_table
{
	struct page pages[1024];
}

struct page_directory
{
	page_table_t *tables[1024];
	uint32_t tables_physical[1024];
	uint32_t physical_addr;
}

void initiallize_paging();

void switch_page_directory(page_directory_t *new_dir);

struct page *get_page(uint32_t address, int make, struct page_directory *dir);

void page_fault(struct registers regs);
