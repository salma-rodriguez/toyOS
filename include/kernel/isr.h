#ifndef _ISR_H_
#define _ISR_H_

#include <asm/int_defs.h>
#include <kernel/types.h>

#define SIG_DZEROV	INT0
#define SIG_DEBUGV	INT1
#define SIG_NMINTV	INT2
#define SIG_BPOINT	INT3
#define SIG_OVRFLO	INT4
#define SIG_BOUNDS	INT5
#define SIG_OPCODE	INT6
#define SIG_NAVAIL	INT7
#define SIG_DOUBLE	INT8
#define SIG_SEGOVR	INT9
#define SIG_SEGINV	INT10
#define SIG_SEGNPR	INT11
#define SIG_STACKV	INT12
#define SIG_GENPFV	INT13
#define SIG_PAGEFV	INT14
#define SIG_SEGFLT	INT15
#define SIG_MATHFV	INT16

#define PIT		IRQ0
#define KEYBOARD	IRQ1
#define CASCADE		IRQ2
#define COM2		IRQ3
#define	COM1		IRQ4
#define LPT2		IRQ5
#define FLOPPY		IRQ6
#define LPT1		IRQ7
#define CMOS		IRQ8
#define FREE1		IRQ9
#define FREE2		IRQ10
#define FREE3		IRQ11
#define PS2_MOUSE	IRQ12
#define FPU		IRQ13
#define PATA_IDE	IRQ14
#define SATA_IDE	IRQ15

struct registers
{
	uint32_t ds;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t int_no;
	uint32_t err_code;
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t useresp;
	uint32_t ss;
};

typedef void (*isr_t)(struct registers *);
void register_interrupt_handler(uint8_t n, isr_t handler);

#endif /* _ISR_H_ */
