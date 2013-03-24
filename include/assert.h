#ifndef _ASSERT_H_
#define _ASSERT_H_

#include <

#define ASSERT(cond) \
        if (!cond) BUG("condition not met")

#endif /* _ASSERT_H_ */
