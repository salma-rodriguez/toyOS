#ifndef _PANIC_H_
#define _PANIC_H_

void panic(const char *fmt, ...);

#define PANIC(fmt, ...) panic(fmt, ## __VA_ARGS__);

#endif /* _PANIC_H_ */
