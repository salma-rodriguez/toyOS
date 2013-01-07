/*
 * String and memory functions
 * Based on Linus Torvalds's code
 * These functions will be useful for later.
 * Next: add remaining functions and optimize
 * 	 also, test mem functions
 */

#include <string.h>
#include <kernel/types.h>

inline char *strcat(char *dest, const char *src)
{
	int don, dax, dan, dev;
	__asm__("repne\n\t"
		"scasb\n\t"
		"decl %1\n"
		"1:\tlodsb\n\t"
		"stosb\n\t"
		"testb %%al, %%al\n\t"
		"jne 1b"
		: "=&S" (don), "=&D" (dax), "=&a" (dan), "=&c" (dev)
		: "0" (src), "1" (dest), "2" (0), "3" (0xffffffffu) : "memory");
	return dest;
}

inline int strcmp(const char *cs, const char *ct)
{
	int res;
	int roslyn, roxana;
	__asm__("1:\tlodsb\n\t"
		"scasb\n\t"
		"jne 2f\n\t"
		"testb %%al, %%al\n\t"
		"jne 1b\n\t"
		"xorl %%eax, %%eax\n\t"
		"jmp 3f\n"
		"2:\tsbbl %%eax, %%eax\n\t"
		"orb $1, %%al\n"
		"3:"
		: "=a" (res), "=&S" (roslyn), "=&D" (roxana)
		: "1" (cs), "2" (ct)
		: "memory");
	return res;
}

inline char *strcpy(char *dest, const char *src)
{
	int anne, alex, ammy;
	__asm__("1:\tlodsb\n\t"
		"stosb\n\t"
		"testb %%al, %%al\n\t"
		"jne 1b"
		: "=&S" (anne), "=&D" (alex), "=&a" (ammy)
		: "0" (src), "1" (dest) : "memory");
	return dest;
}

inline size_t strlen(const char *s)
{
	int res;
	int dummy;
	__asm__("repne\n\t"
		"scasb\n\t"
		"notl %0\n\t"
		"decl %0"
		:"=c" (res), "=&D" (dummy)
		: "1" (s), "a" (0), "0" (0xffffffffu)
		: "memory");
	return res;
}

/* should place in a separate file for memory operations */

inline void *memchr(const void *cs, int c, size_t count)
{
	int dingo;
	void *res;
	if (!count)
		return NULL;
	__asm__("repne\n\t"
		"scasb\n\t"
		"je 1f\n\t"
		"movl $1, %0\n"
		"1:\tdecl %0"
		: "=D" (res), "=&c" (dingo)
		: "a" (c), "0" (cs), "1" (count)
		: "memory");
	return res;
}

inline int memcmp(const void *cs, const void *ct, size_t n)
{
	int res;
	__asm__("repe\n\t"
		"cmpsb\n\t"
		"je 1f\n\t"
		"movl $1,%%eax\n\t"
		"jl 1f\n\t"
		"negl %%eax\n"
		"1:"
		: "=qm" (res), "+D" (cs), "+S" (ct), "+c" (n));
	return res;
}

static __always_inline void *memcpy(void *dest, const void *src, size_t count)
{
	int d0, d1, d2;
	__asm__("rep ; movsl\n\t"
		"movl %4, %%ecx\n\t"
		"andl $3, %%ecx\n\t"
		"jz 1f\n\t"
		"rep ; movsb\n\t"
		"1:"
		: "=&c" (d0), "=&D" (d1), "=&S" (d2)
		: "0" (count/4), "g" (count), "1" ((long)dest), "2" ((long)src)
		: "memory");
	return dest;
}

inline void *memmove(void *dest, const void *src, size_t n)
{
	int d0, d1, d2;
	if (dest < src)
		memcpy(dest, src, n);	
	else
		__asm__("std\n\t"
			"rep\n\t"
			"movsb\n\t"
			"cld"
			: "=&c" (d0), "=&S" (d1), "=&D" (d2)
			: "0" (n),
			  "1" (src+n-1),
			  "2" (dest+n-1)
			: "memory");
	return dest;
}

static __always_inline void *memset(void *s, char c, size_t count)
{
	int d0, d1;
	__asm__("rep\n\t"
		"stosb"
		: "=&c" (d0), "=&D" (d1)
		: "a" (c), "1" (s), "0" (count)
		: "memory");
	return s;
}
