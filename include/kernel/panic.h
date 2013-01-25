#ifndef _PANIC_H_
#define _PANIC_H_

#define PANIC(fmt, ...) panic(fmt, ## __VA_ARGS__);

void panic(const char *fmt, ...);

#endif /* _PANIC_H_ */
