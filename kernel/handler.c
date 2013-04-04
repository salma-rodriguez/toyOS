#ifndef _HANDLER_H_
#define _HANDLER_H_

#include <kernel/isr.h>
#include <kernel/printk.h>
#include <kernel/panic.h>

/** handlers for common interrupts **/

/* these are scattered in different files
 * but still initialized here; most don't do
 * anything other than print to the screen,
 * but still useful for debugging.
 */

void handle_divz_fault(struct registers *regs)
{
	printk("division by zero!\n");
}

void handle_debugger(struct registers *regs)
{
	printk("debug exception!\n");
}

void handle_non_maskable(struct registers *regs)
{
	printk("non-maskable interrupt was triggered\n");
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
	PANIC("out of bounds exception!");
}

void handle_inv_opcode(struct registers *regs)
{
	printk("invalid opcode exception!\n");
}

void handle_co_pnavail(struct registers *regs)
{
	printk("co-processor is not available!\n");
}

void handle_double_fault(struct registers *regs)
{
	printk("double fault!");
	PANIC("error code: 0x%lx\n", regs->err_code);
}

void handle_seg_overrun(struct registers *regs)
{
	printk("co-processor segment overrun!\n");
}

void handle_seg_inv(struct registers *regs)
{
	printk("invalid segment (bad TSS)!\n");
	PANIC("error code: 0x%lx\n", regs->err_code);
}

void handle_seg_np_fault(struct registers *regs)
{
	printk("segment not present!\n");
	PANIC("error code: 0x%lx\n", regs->err_code);
}

void handle_stack_fault(struct registers *regs)
{
	printk("a stack fault has occurred!\n");
	PANIC("error code: 0x%lx\n", regs->err_code);
}

void handle_gpf(struct registers *regs)
{
	printk("general protection fault!\n");
	PANIC("error code: 0x%lx\n", regs->err_code);
}

extern void handle_page_fault(struct registers *regs);

void handle_unknown(struct registers *regs)
{
	printk("unknown exception\n");
}

void handle_math_fault(struct registers *regs)
{
	printk("co-processor fault!\n");
}

void handle_align_check(struct registers *regs)
{
	printk("alignment check exception!\n");
}

void handle_machine_chck(struct registers *regs)
{
	printk("machine check exception!\n");
}

extern void handle_pit_irq(struct registers *regs);

extern void handle_keyboard_irq(struct registers *regs);

void handle_cascade_irq(struct registers *regs)
{
	printk("cascade handler not implemented\n", regs->int_no);
}

void handle_com2_irq(struct registers *regs)
{
	printk("serial port COM2 handler not implemented\n");
}

void handle_com1_irq(struct registers *regs)
{
	printk("serial port COM1 handler not implemented\n");
}

void handle_lpt2_irq(struct registers *regs)
{
	printk("lpt2 interrupt handler not implemented yet\n");
}

void handle_floppy_irq(struct registers *regs)
{
	printk("floppy interrupt handler not implemented\n");
}

void handle_lpt1_irq(struct registers *regs)
{
	printk("lpt1 interrupt handler not implemented\n");
}

void handle_cmos_irq(struct registers *regs)
{
	printk("CMOS real-time clock handler not implemented\n");
}

void handle_free1_irq(struct registers *regs)
{
	printk("first open interrupt handler not implemented\n");
}

void handle_free2_irq(struct registers *regs)
{
	printk("second open interrupt handler not implemented\n");
}

void handle_free3_irq(struct registers *regs)
{
	printk("third open interrupt handler not implemented\n");
}

void handle_mouse_irq(struct registers *regs)
{
	printk("mouse handler not implemented\n");
}

void handle_fpu_irq(struct registers *regs)
{
	printk("floating point unit handler not implemented\n");
}

void handle_phdd_irq(struct registers *regs)
{
	printk("first IDE channel handler not implemented\n");
}

void handle_shdd_irq(struct registers *regs)
{
	printk("second IDE channel handler not implemented\n");
}

void init_irq_handlers()
{
	DPRINTK("IRQ handlers...\t\t");

	register_interrupt_handler(PIT, 	(isr_t)handle_pit_irq);
	register_interrupt_handler(KEYBOARD, 	(isr_t)handle_keyboard_irq);
	register_interrupt_handler(CASCADE,	(isr_t)handle_cascade_irq);
	register_interrupt_handler(COM2,	(isr_t)handle_com2_irq);
	register_interrupt_handler(COM1,	(isr_t)handle_com1_irq);
	register_interrupt_handler(LPT2,	(isr_t)handle_lpt2_irq);
	register_interrupt_handler(FLOPPY,	(isr_t)handle_floppy_irq);
	register_interrupt_handler(LPT1,	(isr_t)handle_lpt1_irq);
	register_interrupt_handler(CMOS,	(isr_t)handle_cmos_irq);
	register_interrupt_handler(FREE1,	(isr_t)handle_free1_irq);
	register_interrupt_handler(FREE2, 	(isr_t)handle_free2_irq);
	register_interrupt_handler(FREE3,	(isr_t)handle_free3_irq);
	register_interrupt_handler(PS2_MOUSE,	(isr_t)handle_mouse_irq);
	register_interrupt_handler(FPU,		(isr_t)handle_fpu_irq);
	register_interrupt_handler(PATA_IDE,	(isr_t)handle_phdd_irq);
	register_interrupt_handler(SATA_IDE,	(isr_t)handle_shdd_irq);

	printk("done!\n");
}

void init_fault_handlers()
{
	DPRINTK("fault handlers...\t");

	register_interrupt_handler(SIG_DZEROV, (isr_t)handle_divz_fault);
	register_interrupt_handler(SIG_DEBUGV, (isr_t)handle_debugger);
	register_interrupt_handler(SIG_NMINTV, (isr_t)handle_non_maskable);
	register_interrupt_handler(SIG_BPOINT, (isr_t)handle_breakpoint);
	register_interrupt_handler(SIG_OVRFLO, (isr_t)handle_overflow);
	register_interrupt_handler(SIG_BOUNDS, (isr_t)handle_bounds_err);
	register_interrupt_handler(SIG_OPCODE, (isr_t)handle_inv_opcode);
	register_interrupt_handler(SIG_NAVAIL, (isr_t)handle_co_pnavail);
	register_interrupt_handler(SIG_DOUBLE, (isr_t)handle_double_fault);
	register_interrupt_handler(SIG_SEGOVR, (isr_t)handle_seg_overrun);
	register_interrupt_handler(SIG_SEGINV, (isr_t)handle_seg_inv);
	register_interrupt_handler(SIG_SEGNPR, (isr_t)handle_seg_np_fault);
	register_interrupt_handler(SIG_STACKV, (isr_t)handle_stack_fault);
	register_interrupt_handler(SIG_GENPFV, (isr_t)handle_gpf);
	register_interrupt_handler(SIG_PAGEFV, (isr_t)handle_page_fault);
	register_interrupt_handler(SIG_UNKNWN, (isr_t)handle_unknown);
	register_interrupt_handler(SIG_MATHFV, (isr_t)handle_math_fault);
	register_interrupt_handler(SIG_ALGMNT, (isr_t)handle_align_check);
	register_interrupt_handler(SIG_MACHNE, (isr_t)handle_machine_chck);

	DPRINTK("done!\n");
}

#endif /* _HANDLER_H_ */
