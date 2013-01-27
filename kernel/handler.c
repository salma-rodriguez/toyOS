#ifndef _HANDLER_H_
#define _HANDLER_H_

#include <kernel/isr.h>
#include <kernel/printk.h>
#include <kernel/panic.h>

extern isr_t interrupt_handlers[INTS];

extern void handle_pit_irq(struct registers *regs);
extern void handle_page_fault(struct registers *regs);

void handle_divz_fault(struct registers *regs)
{
	printk("devision by zero!\n");
}

void handle_breakpoint(struct registers *regs)
{
	printk("breakpoint triggered at: %lx\n", regs->eip);
}

void handle_overflow(struct registers *regs)
{
	printk("operation overflow\n");
}

void handle_bounds_err(struct registers *regs)
{
	PANIC("A bounds error has occurred!");
}

void handle_inv_opcode(struct registers *regs)
{
	printk("invalid opcode!\n");
}

void handle_gpf(struct registers *regs)
{
	PANIC("global protection fault!\n");
}

void handle_seg_fault(struct registers *regs)
{
	printk("segmentation fault!\n");
}

void handle_double_fault(struct registers *regs)
{
	PANIC("double fault!");
}

void init_irq_handlers()
{
	printk("IRQ handlers...\t\t");

	register_interrupt_handler(PIT, (isr_t)handle_pit_irq);
	
	printk("done!\n");
}

void init_fault_handlers()
{
	printk("fault handlers...\t");

	register_interrupt_handler(SIG_BPOINT, (isr_t)handle_breakpoint);
	register_interrupt_handler(SIG_BOUNDS, (isr_t)handle_bounds_err);
	register_interrupt_handler(SIG_OPCODE, (isr_t)handle_inv_opcode);
	register_interrupt_handler(SIG_GENPFV, (isr_t)handle_gpf);
	register_interrupt_handler(SIG_DZEROV, (isr_t)handle_divz_fault);
	register_interrupt_handler(SIG_SEGFLT, (isr_t)handle_seg_fault);
	register_interrupt_handler(SIG_PAGEFV, (isr_t)handle_page_fault);
	register_interrupt_handler(SIG_DOUBLE, (isr_t)handle_double_fault);

	printk("done!\n");
}

#endif /* _HANDLER_H_ */
