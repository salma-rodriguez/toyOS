#ifndef _ASSERT_H_
#define _ASSERT_H_

#include <kernel/bug.h>

#define ASSERT(cond) \
        if (!(cond)) BUG("condition not met")

#endif /* _ASSERT_H_ */
