#include <stdio.h>
#include <kernel/types.h>
#include <kernel/common.h>
#include <kernel/isr.h>
#include <kernel/timer.h>

uint32_t tick = 0;

void timer_callback(struct registers regs)
{
	tick++;
	printf("Tick: %d\n", tick);
}

void init_timer(uint32_t frequency)
{
	uint8_t l, h;
	uint32_t divisor;

	register_interrupt_handler(CLOCK, &timer_callback);

	// Value sent to the PIT is to divide input clock
	// (1193180 Hz) by to get frequency. Important note:
	// divisor must be small enough to fit into 16 bits.
	divisor = 1193180 / frequency;

	// Send the command byte.
	outb(0x43, 0x36);

	// Divisor has to be sent byte-wise; split into upper/lower
	l = (uint8_t)(divisor & 0xFF);
	h = (uint8_t)((divisor>>8) & 0xFF);

	// Send the frequency divisor.
	outb(0x40, l);
	outb(0x40, h);
}
