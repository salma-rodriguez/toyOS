#include <stdio.h>
#include <asm/isr.h>
#include <kernel/common.h>

void isr_handler(registers_t regs)
{
	printf("recieved interrupt: %d\n", regs.int_no);
}
