#include <asm/common.h>
#include <kernel/types.h>
#include <kernel/isr.h>
#include <kernel/timer.h>
#include <kernel/printk.h>

#define CLOCK IRQ0

uint32_t tick;

void timer_callback(struct registers regs)
{
	tick++;
	DPRINTK("Tick: %d\n", tick);
}

void init_timer(uint32_t frequency)
{
	uint8_t l, h;
	uint32_t divisor;

	DPRINTK("timer...\t");

	tick = 0;
	register_interrupt_handler(CLOCK, (isr_t)timer_callback);

	divisor = 1193180 / frequency;
	outportb(0x43, 0x36);

	l = (uint8_t)(divisor & 0xFF);
	h = (uint8_t)((divisor>>8) & 0xFF);

	outportb(0x40, l);
	outportb(0x40, h);

	DPRINTK("done!\n");
}
