#ifndef STRING_H_
#define STRING_H_

#ifndef NULL
#define NULL ((void *) 0)
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

#ifndef __asm__
#define __asm__ asm volatile
#endif

extern inline int strlen(const char * s);
extern inline int strcmp(const char *cs, const char *ct);
extern inline char * strcat(char * dest, const char * src);
extern inline char * strcpy(char * dest, const char * src);

#endif
