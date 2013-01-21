#include <stdio.h>
#include <asm/common.h>
#include <kernel/isr.h>
#include <kernel/types.h>

isr_t interrupt_handlers[256];

void isr_handler(struct registers regs)
{
	isr_t handler;

	if (regs.int_no != 13)
		printf("recieved interrupt: %d\n", regs.int_no);

	if (interrupt_handlers[regs.int_no])
	{
		handler = interrupt_handlers[regs.int_no];
		handler(&regs);
	}
}

void irq_handler(struct registers regs)
{
	isr_t handler;

	// Send an EOI signal to PICs

	// If this interrupt involved the slave:
	if (regs.int_no >= 40)
		outportb(0xA0, 0x20);

	// Send reset signal to master, regardless.
	outportb(0x20, 0x20);

	if (interrupt_handlers[regs.int_no])
	{
		handler = interrupt_handlers[regs.int_no];
		handler(&regs);
	}
}

void register_interrupt_handler(uint8_t n, isr_t handler)
{
	if (interrupt_handlers[n])
		printf("Overriding interrupt handler: %d\n.", n);
	interrupt_handlers[n] = handler;
}
