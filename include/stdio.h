#ifndef _STDIO_H_
#define _STDIO_H_

#include <stdarg.h>

extern int vsprintf(char *buf, const char *fmt, va_list args);
extern int sprintf(char *buf, const char *fmt, ...);
extern int printf(const char *fmt, ...);

#endif
