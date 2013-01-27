#ifndef _PRINTK_H_
#define _PRINTK_H_

#include <stdio.h>

#define DEBUG 0

#define printk(s, args...) printf(s, ##args)

#ifdef DEBUG

#if DEBUG
#define DPRINTK(s, args...) printk(s, ##args)
#else
#define DPRINTK(s, args...)
#endif

#endif /* DEBUG */

#endif /* _PRINTK_H_ */
