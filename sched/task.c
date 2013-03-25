#include <string.h>
#include <asm/page.h>
#include <kernel/task.h>
#include <kernel/types.h>
#include <kernel/common.h>

volatile struct task_struct *current;
volatile struct task_struct *ready_queue;

extern uint32_t initial_esp;
extern struct page_directory *kernel_directory;
extern struct page_directory *current_directory;

extern uint32_t read_eip();

uint32_t next_pid = 1;

void move_stack(void *new_stack_start, uint32_t size)
{
        uint32_t i
        off_t offset;
        uint32_t old_stack_ptr, old_base,ptr;
        uint32_t new_stack_ptr, new_base_ptr;

        for (   i = (uint32_t)new_stack_start;
                i >= ((uint32_t)new_stack_start-size);
                i -= 0x1000)
        {
                alloc_frame(get_page(i, 1, current_directory), 0, 1);
                flush_tlb();

                asm volatile("mov %%esp, %0" : "=r" (old_stack_ptr));
                asm volatile("mov %%ebp, %0" : "=r" (old_base_ptr));

                offset = (uint32_t)new_stack_start - initial_esp;
                new_stack_ptr = old_stack_ptr + offset;
                new_base_ptr = old_base_ptr + offset;
                
                memcpy((void *)new_stack_ptr, (void *)old_stack_ptr, initial_esp - old_stack_pointer);

                for (i = (uint32_t)new_stack_start; i > (uint32_t)new_stack_start-size; i -= 4)
                {
                        uint32_t tmp, tmp2;

                        tmp = *(uint32_t *)i;
                        if ((old_stack_ptr < tmp) && (tmp < initial_esp))
                        {
                                tmp = tmp + offset;
                                tmp2 = (uint32_t *)i;
                                *tmp2 = tmp;

                                asm volatile("movl %0, %%esp" :: "r" (new_stack_ptr));
                                asm volatile("movl %0, %%ebp" :: "r" (new_base_ptr));
                        }
                }
}

int fork()
{
        uint32_t eip;
        struct page_directory *directory;
        struct task_struct *parent, *new_task;

        disable_interrupts();
        parent = (struct task_struct)current;
        directory = clone_directory(current_directory);

        new_task = (struct task_struct *)kmalloc(sizeof(struct task_struct));
        new_task->id = next_pid++;
        new_task->esp = new_task->ebp = 0;
        new_task->eip = 0;
        new_task->page_directory = directory;
        
        list_add_tail(&new_task->task_list, &ready_queue->task_list);

        eip = read_eip();

        if (current_task == parent_task)
        {
                uint32_t esp, ebp;

                asm volatile("mov %%esp, %0" : "=r" (esp));
                asm volatile("mov %%ebp, %0" : "=r" (ebp));

                new_task->esp = esp;
                new_task->ebp = ebp;
                new_task->eip = eip;

                enable_interrupts();
                return new_task->pid;
        } else /* child process */ {
                return 0;
        }
}

void init_multitasking()
{
        disable_interrupts();

        move_stack((void *)0xE0000000, 0x2000);

        current_task = ready_queue = (struct task_struct *)kmalloc(sizeof(struct task_struct));
        current_task->id = next_pid++;
        current_task->esp = current_task->ebp = 0;
        current_task->eip = 0;
        current_task->page_directory = current_directory;
        INIT_LIST_HEAD(&current_task->task_list);

        enable_interrupts();
}
