#include <stdio.h>
#include <kernel/isr.h>
#include <kernel/types.h>
#include <kernel/common.h>

isr_t interrupt_handlers[256];

void isr_handler(registers_t regs)
{
	printf("recieved interrupt: %d\n", regs.int_no);

	if (interrupt_handlers[regs.int_no] != 0)
	{
		isr_t handler = interrupt_handlers[regs.int_no];
		handler(&regs);
	}
}

void register_interrupt_handler(uint8_t n, isr_t handler)
{
	interrupt_handlers[n] = handler;
}

void irq_handler(registers_t regs)
{
	// Send an EOI signal to PICs.
	// If this interrupt involved the slave:
	if (regs.int_no >= 40)
		outb(0xA0, 0x20);
	// Send reset signal to master, regardless.
	outb(0x20, 0x20);

	if (interrupt_handlers[regs.int_no])
	{
		isr_t handler = interrupt_handlers[regs.int_no];
		handler(&regs);
	}
}
