#include <asm/common.h>
#include <kernel/types.h>
#include <kernel/isr.h>
#include <kernel/task.h>
#include <kernel/timer.h>
#include <kernel/printk.h>

uint32_t tick;

void handle_pit_irq(struct registers *regs)
{
	tick++;
	DPRINTK("Tick: %d\n", tick);
	switch_context();
}

void init_timer(uint32_t frequency)
{
	uint8_t l, h;
	uint32_t divisor;

        DPRINTK("timer...\t\t");

	tick = 0;

	divisor = 1193180 / frequency;
	outportb(0x43, 0x36);

	l = (uint8_t)(divisor & 0xFF);
	h = (uint8_t)((divisor>>8) & 0xFF);

	outportb(0x40, l);
	outportb(0x40, h);

	DPRINTK("done!\n");
}
