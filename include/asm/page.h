#ifndef _PAGE_H_
#define _PAGE_H_

#include <asm/common.h>
#include <kernel/isr.h>
#include <kernel/types.h>

#define PAGE_SIZ 1 << 12
#define PAGING_FLAG 0x80000000

struct page
{
	uint32_t present	: 1;
	uint32_t rw		: 1;
	uint32_t user		: 1;
	uint32_t accessed	: 1;
	uint32_t dirty		: 1;
	uint32_t unused		: 7;
	uint32_t frame		: 20;
};

struct page_table
{
	struct page pages[1024];
};

struct page_directory
{
	struct page_table *tables[1024];
	uint32_t tables_physical[1024];
	uint32_t physical_addr;
};

static inline void enable_paging()
{
	register int paging_flag;
	paging_flag = PAGING_FLAG;
	__asm__ __volatile__ ("movl %%cr0, %%eax\n\t"
			      "orl %0, %%eax\n\t"
			      "movl %%eax, %%cr0"
			      :: "r" (paging_flag));
}

static inline __u32 get_faulting_address()
{
	__u32 faulting_address;
	__asm__ __volatile__ ("mov %%cr2, %0" : "=r" (faulting_address));
	return faulting_address;
}

static inline void switch_page_directory(struct page_directory *dir)
{
	__asm__ __volatile__ ("mov %0, %%cr3" :: "r" (dir->tables_physical));
}

static inline void flush_tlb_single(__u32 addr)
{
	__asm__ __volatile__ ("invlpg (%0)" :: "r" (addr) : "memory");
}

static inline void flush_tlb()
{
	__u32 page_dir_addr;
	__asm__ __volatile__ ("mov %%cr3, %0" : "=r" (page_dir_addr));
	__asm__ __volatile__ ("mov %0, %%cr3" :: "r" (page_dir_addr));
}


extern struct page_directory *current_directory;

void init_paging();
void handle_page_fault(struct registers regs);
struct page *get_page(uint32_t address, int creat, struct page_directory *dir);

#endif /* _PAGE_H_ */