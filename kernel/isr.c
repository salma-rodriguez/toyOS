#include <asm/common.h>
#include <kernel/isr.h>
#include <kernel/types.h>
#include <kernel/printk.h>

isr_t interrupt_handlers[INTS];

void isr_handler(struct registers regs)
{
	isr_t handler;

	DPRINTK("recieved interrupt: %d\n", regs.int_no);

	send_eoi_master();

	if (interrupt_handlers[regs.int_no])
	{
		handler = interrupt_handlers[regs.int_no];
		handler(&regs);
	}
}

void irq_handler(struct registers regs)
{
	isr_t handler;

	if (regs.int_no >= 40)
		send_eoi_slave();

	send_eoi_master();

	if (interrupt_handlers[regs.int_no])
	{
		handler = interrupt_handlers[regs.int_no];
		handler(&regs);
	}
}

void register_interrupt_handler(uint8_t n, isr_t handler)
{
	if (interrupt_handlers[n])
		printk("overriding interrupt handler: %d\n.", n);
	interrupt_handlers[n] = handler;
}
