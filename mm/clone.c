#include <asm/page.h>

extern void copy_page_physical(uint32_t, uint32_t);

static struct page_table *clone_table(struct page_table *src, uint32_t *phys_addr)
{
        int i;
        struct page_table *table;

        table = (struct page_table *)kmalloc_ap(sizeof(struct page_table), phys_addr);
        memset(table, 0, sizeof(struct page_directory));

        for (i = 0; i < 1024; i++)
        {
                if (!src->pages[i].frame)
                        continue;
                alloc_frame(&table->pages[i], 0, 0);

                if (src->pages[i].present) table->pages[i].present = 1;
                if (src->pages[i].rw) table->pages[i].rw = 1;
                if (src->pages[i].user) table->pages[i].user = 1;
                if (src->pages[i].accessed) table->pages[i].accessed = 1;
                if (src->pages[i].dirty) table->pages[i].dirty = 1;

                copy_page_physical(src->pages[i].frame*0x1000, table->pages[i].frame*0x1000);
        }
        return table;
}

struct page_directory *clone_directory(struct page_directory *src)
{
        int i;
        off_t offset;
        uint32_t phys;

        struct page_directory *dir = (struct page_directory *)kmalloc_ap(sizeof(struct page_directory), &phys);
        memset(dir, 0, sizeof(struct page_directory));
        
        offset = (off_t)(dir->tables_physical - dir);
        dir->physicalAddr = phys + offset;

        for (i = 0; i < 1024; i++)
        {
                if (!src->tables[i])
                        continue;
                if (kernel_directory->tables[i] == src->tables[i]) {
                        dir->tables[i] = src->tables[i];
                        dir->tables_physical[i] = src->tables_physical[i];
                } else {
                        dir->tables[i] = clone_table(src->tables[i], &phys);
                        dir->tables_physical[i] = phys | 0x07;
                }
        }
        return dir;
}
