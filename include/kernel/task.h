#ifndef _TASK_H_
#define _TASK_H_

#include <list.h>
#include <asm/page.h>
#include <asm/common.h>

struct task_struct
{
        int pid;
        uint32_t eip;
        uint32_t esp, ebp;
        struct page_directory *page_directory;
        struct list_head task_list;
};

void init_multitasking();
void switch_context();
int fork();
void move_stack(void *new_stack_start, uint32_t size);
int getpid();

#endif /* _TASK_H_ */
