#ifndef _HANDLER_H_
#define _HANDLER_H_

void init_irq_handlers();
void init_fault_handlers();

/** function prototypes for common interrupts **/

/* these are scattered in different files
 * but still referenced from here; most don't do
 * anything other than print to the screen, but
 * still useful for debugging... for now.
 */

extern void handle_pit_irq(struct registers *);
extern void handle_keyboard_irq(struct registers *);
extern void handle_cascade_irq(struct registers *);
extern void handle_com1_irq(struct registers *);
extern void handle_com2_irq(struct registers *);
extern void handle_lpt1_irq(struct registers *);
extern void handle_lpt2_irq(struct registers *);
extern void handle_floppy_irq(struct registers *);
extern void handle_cmos_irq(struct registers *);
extern void handle_free1_irq(struct registers *);
extern void handle_free2_irq(struct registers *);
extern void handle_free3_irq(struct registers *);
extern void handle_mouse_irq(struct registers *);
extern void handle_fpu_irq(struct registers *);
extern void handle_phdd_irq(struct registers *);
extern void handle_shdd_irq(struct registers *);

extern void handle_divz_fault(struct registers *);
extern void handle_debugger(struct registers *);
extern void handle_non_maskable(struct registers *);
extern void handle_breakpoint(struct registers *);
extern void handle_overflow(struct registers *);
extern void handle_bounds_err(struct registers *);
extern void handle_inv_opcode(struct registers *);
extern void handle_co_pnavail(struct registers *);
extern void handle_double_fault(struct registers *);
extern void handle_seg_overrun(struct registers *);
extern void handle_seg_inv(struct registers *);
extern void handle_seg_np_fault(struct registers *);
extern void handle_stack_fault(struct registers *);
extern void handle_gpf(struct registers *);
extern void handle_page_fault(struct registers *);
extern void handle_seg_fault(struct registers *);
extern void handle_math_fault(struct registers *);

#endif /* _HANDLER_H_ */
