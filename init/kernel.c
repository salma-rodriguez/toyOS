#include "multiboot.h"
#include <asm/page.h>
#include <asm/common.h>
#include <kernel/isr.h>
#include <kernel/types.h>
#include <kernel/timer.h>
#include <kernel/panic.h>
#include <kernel/monitor.h>
#include <kernel/printk.h>
#include <kernel/handler.h>
#include <kernel/tables.h>
#include <kernel/heap.h>
#include <kernel/fs.h>
#include <kernel/initrd.h>

extern struct heap *kheap;
extern uint32_t placement_addr;

int kmain(multiboot_info_t *mbd, uint32_t magic)
{
        uint8_t buf[256];
        struct dirent *node;
        struct fs_node *fsnode;
	uint32_t a, b, c, d, i, j, sz;
	uint32_t initrd_location, initrd_end;

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) 
	{
		PANIC("boot magic is incorrect\n");
	}

	monitor_clear();

	printk("initializing...\n");

        // disable_interrupts();

	init_descriptor_tables();
	
        initrd_location = *((uint32_t *)mbd->mods_addr);
        initrd_end = *(uint32_t *)(mbd->mods_addr+4);
        placement_addr = initrd_end;

        init_irq_handlers();
        init_fault_handlers();

        init_timer(PIT_FREQUENCY);

	// a = kmalloc(8);

	init_paging();

        // enable_interrupts();

        printk("after initializing paging and all that\n");

	fs_root = (struct fs_node *)initialize_initrd(initrd_location);

        /* testing vfs { */

                i = 0;
                node = NULL;
                while ((node = readdir_fs(fs_root, i)) != 0)
                {
                        monitor_write("Found file ");
                        monitor_write(node->name);
                        fsnode = finddir_fs(fs_root, node->name);
                        if ((fsnode->flags&0x7) == FS_DIRECTORY)
                                printk("\n\t(directory)\n");
                        else
                        {
                                printk("\n\t contents: \"");
                                sz = read_fs(fsnode, 0, 256, buf);
                                for (j = 0; j < sz; j++)
                                        printk("%c", buf[j]);
                                printk("\"\n");
                        }
                        i++;
                }

        /* } */

        /* testing memory allocation { */

               //  b = kmalloc(8);
               //  c = kmalloc(8);
               // 
               //  printk("a: %lx, b: %lx\nc: %lx", a, b, c);
               // 
               //  kfree((void *)c);
               //  kfree((void *)b);
               // 
               //  d = kmalloc(12);
               // 
               //  printk(", d: %lx\n", d);

        /* } */

        /* testing division by zero { */

                // int i;
                // i = 500 / 0;
                // testing page fault
                /* uint32_t *ptr = (uint32_t *)0xA0000000;
                uint32_t do_page_fault = *ptr; */

        /* } */

	while(1);

	return 0;
}
