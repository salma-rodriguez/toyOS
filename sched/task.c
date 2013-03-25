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

        if (current == parent_task)
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

void switch_context()
{
        if (!current)
                return;

        uint32_t esp, ebp, eip;

        asm volatile("mov %%esp, %0" : "=r" (esp));
        asm volatile("mov %%ebp, %0" : "=r" (ebp));

        eip = read_eip();

        if (eip == 0x12345)
                return;

        current->eip = eip;
        current->esp = esp;
        current->ebp = ebp;

        current = list_entry(current->task_list.next, struct task_struct, task_list);

        esp = current->esp;
        ebp = current->ebp;

        asm volatile("                  \
                cli;                    \
                mov %0, %%ecx;          \
                mov %1, %%esp;          \
                mov %2, %%ebp;          \
                mov %3, %%cr3;          \
                mov $0x12345, %%eax;    \
                sti;                    \
                jmp *%%ecx              "
                :: "r" (eip), "r" (esp), "r" (ebp), "r" (current_directory->physical_addr)
        );
}

void init_multitasking()
{
        disable_interrupts();

        move_stack((void *)0xE0000000, 0x2000);

        current = ready_queue = (struct task_struct *)kmalloc(sizeof(struct task_struct));
        current->id = next_pid++;
        current->esp = current->ebp = 0;
        current->eip = 0;
        current->page_directory = current_directory;
        INIT_LIST_HEAD(&current->task_list);

        enable_interrupts();
}
