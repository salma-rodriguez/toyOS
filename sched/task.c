#include <string.h>
#include <asm/page.h>
#include <kernel/types.h>

extern initial_esp;

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
