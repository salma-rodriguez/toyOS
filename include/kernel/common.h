#include <kernel/types.h>

#ifndef _COMMON_H_
#define _COMMON_H_

#ifndef SEGMENT_FLAGS
#define SEGMENT_FLAGS

#define CODE				0x0A
#define DATA				0x02

#endif /* SEGMENT_FLAGS */

#ifndef ACCESS_FLAGS
#define ACCESS_FLAGS

#define RING_0_MASK			0 << 4
#define RING_1_MASK			2 << 4
#define Ring_2_MASK			4 << 4
#define RING_3_MASK			6 << 4

#define P				8 << 4
#define DT				1 << 4
#define DTP				P | DT

#define DPL_0_DTP			RING_0_MASK | DTP
#define DPL_1_DTP			RING_1_MASK | DTP
#define DPL_2_DTP			RING_2_MASK | DTP
#define DPL_3_DTP			RING_3_MASK | DTP

#endif /* ACCESS_FLAGS */

__u8 inb(__u16 port);
__u16 inw(__u16 port);
void outb(__u16 port, __u8 value);

#endif
