#ifndef STRING_H_
#define STRING_H_

#ifndef NULL
#define NULL ((void *) 0)
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

#define __asm__ asm volatile

#ifndef __always_inline
#define __always_inline inline
#endif

extern inline size_t strlen(const char *s);
extern inline int strcmp(const char *cs, const char *ct);
extern inline char *strcat(char *dest, const char *src);
extern inline char *strcpy(char *dest, const char *src);
extern inline void *memchr(const void *cs, int c, size_t count);
extern inline int memcmp(const void *cs, const void *ct, size_t n);
extern void *memmove(void *dest, const void *src, size_t n);
extern __always_inline void *memcpy(void *dest, const void *src, size_t n);
extern __always_inline void *memset(void *s, char c, size_t n);

#endif
