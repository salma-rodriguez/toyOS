#ifndef _STRING_H_
#define _STRING_H_

#ifndef NULL
#define NULL ((void *) 0)
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

#ifndef __always_inline
#define __always_inline inline
#endif

/* str functions */

extern inline char *strcat(char *, const char *);
extern inline char *strlcat(char *, const char *, size_t);
extern inline char *strncat(char *, const char *, size_t);
extern inline char *strchr(const char *, int);
extern inline char *strrchr(const char *, int);
extern inline int strcmp(const char *, const char *);
extern inline int stricmp(const char *, const char *, size_t);
extern inline int strncmp(const char *, const char *, size_t);
extern inline int strcasecmp(const char *, const char *);
extern inline int strncasecmp(const char *, const char *, size_t);
extern inline char *strcpy(char *, const char *);
extern inline char *strlcpy(char *, const char *, size_t);
extern inline char *strncpy(char *, const char *, size_t);
extern inline char *strim(char *);
extern inline size_t strlen(const char *);
extern inline size_t strnlen(const char *, size_t);
extern inline char *strpbrk(const char *, const char *);
extern inline char *strsep(const char **, const char *);
extern inline char *strspn(const char *, const char *);
extern inline char *strcspn(const char *, const char *);
extern inline char *strstr(const char *, const char *);
extern inline char *strnstr(const char *, const char *, size_t);

/* Homework: try implementing
 * strlcat, strrchr, stricmp, strcasecmp, strncasecmp
 * strlcpy, strim, strpbrk, strsep, strspn, strcspn, and
 * strnstr in x86 assembly
 */

/* mem functions */

extern void *memchr(const void *, int, size_t);
extern int memcmp(const void *, const void *, size_t);
extern __always_inline void *memcpy(void *, const void *, size_t);
extern void *memmove(void *, const void *, size_t);
extern void *memscan(void *, int, size_t);
extern __always_inline void *memset(void *, char, size_t);

#endif
