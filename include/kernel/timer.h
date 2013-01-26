#ifndef _TIMER_H_
#define _TIMER_H_

#include <kernel/types.h>

#define PIT_FREQUENCY 50

void init_timer(uint32_t frequency);

#endif
