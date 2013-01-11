#include <kernel/types.h>
#include <kernel/common.h>

typedef struct registers
{
	uint32_t ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no, err_code;

	uint32_t eip, cs, eflags, userresp, ss;
} registers_t;