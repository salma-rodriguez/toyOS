#ifndef _PAGE_H_
#define _PAGE_H_

#include <asm/common.h>
#include <kernel/isr.h>
#include <kernel/types.h>

#define PAGE_SIZ (1 << 12)

#define PRESENT_MASK		(1 << 0)
#define RW_MASK			(1 << 1)
#define USER_MODE_MASK		(1 << 2)
#define RSVD3_MASK		(1 << 3)
#define RSVD4_MASK		(1 << 4)
#define ACCESSED_MASK		(1 << 5)
#define DIRTY_BIT_MASK		(1 << 6)
#define RSVD7_MASK		(1 << 7)
#define RSVD8_MASK		(1 << 8)

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

static inline void enable_pse()
{
	__asm__ __volatile__ ("movl %%cr4, %%eax\n\t"
			      "orl $0x00000010, %%eax\n\t"
			      "movl %%eax, %%cr4" : );
}

static inline void enable_paging()
{
	__asm__ __volatile__ ("movl %%cr0, %%eax\n\t"
			      "orl $0x80000000, %%eax\n\t"
			      "movl %%eax, %%cr0" : );
}

static inline void switch_to_protected_mode()
{
	__asm__ __volatile__ ("movl %%cr0, %%eax\n\t"
			      "orl $0x00000001, %%eax\n\t"
			      "movl %%eax, %%cr0" : );
}

static inline void enable_protected_paging()
{
	__asm__ __volatile__ ("movl %%cr0, %%eax\n\t"
			      "orl $0x80000001, %%eax\n\t"
			      "movl %%eax, %%cr0" : );
}

static inline __u32 get_faulting_address()
{
	__u32 faulting_address;
	__asm__ __volatile__ ("movl %%cr2, %0" : "=r" (faulting_address));
	return faulting_address;
}

static inline void switch_page_directory(struct page_directory *dir)
{
	__asm__ __volatile__ ("movl %0, %%cr3" :: "r" (dir->physical_addr));
}

static inline void flush_tlb_single(__u32 addr)
{
	__asm__ __volatile__ ("invlpg (%0)" :: "r" (addr) : "memory");
}

static inline void flush_tlb()
{
	__u32 page_dir_addr;
	__asm__ __volatile__ ("movl %%cr3, %0" : "=r" (page_dir_addr));
	__asm__ __volatile__ ("movl %0, %%cr3" :: "r" (page_dir_addr));
}

extern struct page_directory *kernel_directory;
extern struct page_directory *current_directory;

void init_paging();
void alloc_frame(struct page *, int, int);
struct page *get_page(uint32_t, int, struct page_directory *);

#endif /* _PAGE_H_ */
