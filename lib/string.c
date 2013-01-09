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

inline char *strncat(char *dest, const char *src, size_t count)
{
	int don, dax, dan, dev;
	__asm__("repne\n\t"
		"scasb\n\t"
		"decl %1\n\t"
		"movl %8, %3\n\t"
		"1:\tdecl %3\n\t"
		"js 2f\n\t"
		"lodsb\n\t"
		"stosb\n\t"
		"testb %%al, %%al\n\t"
		"jne 1b\n"
		"2:\txorl %2, %2\n\t"
		"stosb"
		: "=&S" (don), "=&D" (dax), "=&a" (dan), "=&c" (dev)
		: "0" (src), "1" (dest), "2" (0), "3" (0xffffffffu), "g" (count)
		: "memory");
	return dest;
}

inline char *strchr(const char *s, int c)
{
	int x;
	char *res;
	__asm__("movb %%al, %%ah\n"
		"1:\tlodsb\n\t"
		"cmpb %%ah, %%al\n\t"
		"je 2f\n\t"
		"testb %%al, %%al\n\t"
		"jne 1b\n\t"
		"movl $1, %1\n"
		"2:\tmovl %1, %0\n\t"
		"decl %0"
		: "=a" (res), "=&S" (x)
		: "1" (s), "0" (c)
		: "memory");
	return res;
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

inline int strncmp(const char *cs, const char *ct, size_t count)
{
	int res;
	int roslyn, roxana, rachel;
	__asm__("1:\tdecl %3\n\t"
		"js 2f\n\t"
		"lodsb\n\t"
		"scasb\n\t"
		"jne 3f\n\t"
		"testb %%al, %%al\n\t"
		"jne 1b\n"
		"2:\txorl %%eax, %%eax\n\t"
		"orb $1, %%al\n"
		"4:"
		: "=a" (res), "=&S" (roslyn), "=&D" (roxana), "=&c" (rachel)
		: "1" (cs), "2" (ct), "3" (count)
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

inline char *strncpy(char *dest, const char *src, size_t count)
{
	int anne, alex, ammy, abby;
	__asm__("1:\tdecl %2\n\t"
		"js 2f\n\t"
		"lodsb\n\t"
		"stosb\n\t"
		"testb %%al, %%al\n\t"
		"jne 1b\n\t"
		"rep\n\t"
		"stosb\n"
		"2:"
		: "=&S" (anne), "=&D" (alex), "=&c" (ammy), "=&a" (abby)
		: "0" (src), "1" (dest), "2" (count) : "memory");
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

inline size_t strnlen(const char *s, size_t count)
{
	int res;
	int dummy;
	__asm__("movl %2, %0\n\t"
		"jmp 2f\n"
		"1:\tcmpb $0, (%0)\n\t"
		"je 3f\n\t"
		"incl %0\n"
		"2:\tdecl %1\n\t"
		"cmpl $-1, %1\n\t"
		"jne 1b\n"
		"3:\tsubl %2, %0"
		: "=a" (res), "=&d" (dummy)
		: "c" (s), "1" (count)
		: "memory");
	return res;
}

char *strstr(const char *cs, const char *ct)
{

	char *res;
	int tim, tom;
	__asm__("movl %6, %%edi\n\t"
		"repne\n\t"
		"scasb\n\t"
		"notl %%ecx\n\t"
		"decl %%ecx\n\t"
		"movl %%ecx, %%edx\n"
		"1:\tmovl %6, %%edi\n\t"
		"movl %%esi, %%eax\n\t"
		"movl %%edx, %%ecx\n\t"
		"repe\n\t"
		"cmpsb\n\t"
		"je 2f\n\t"
		"xchgl %%eax, %%esi\n\t"
		"incl %%esi\n\t"
		"cmpb $0, -1(%%eax)\n\t"
		"jne 1b\n\t"
		"xorl %%eax, %%eax\n\t"
		"2:"
		: "=a" (res), "=&c" (tim), "=&S" (tom)
		: "0" (0), "1" (0xffffffff), "2" (cs), "g" (ct)
		: "dx", "di");
	return res;
}

/* should place in a separate file for memory operations */

void *memchr(const void *cs, int c, size_t n)
{
	int d0;
	void *res;
	if (!n)
		return NULL;
	__asm__("repne\n\t"
		"scasb\n\t"
		"je 1f\n\t"
		"movl $1, %0\n"
		"1:\tdecl %0"
		: "=D" (res), "=&c" (d0)
		: "a" (c), "0" (cs), "1" (n)
		: "memory");
	return res;
}

int memcmp(const void *cs, const void *ct, size_t n)
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

__always_inline void *memcpy(void *dest, const void *src, size_t n)
{
	int d0, d1, d2;
	__asm__("rep ; movsl\n\t"
		"movl %4, %%ecx\n\t"
		"andl $3, %%ecx\n\t"
		"jz 1f\n\t"
		"rep ; movsb\n\t"
		"1:"
		: "=&c" (d0), "=&D" (d1), "=&S" (d2)
		: "0" (n/4), "g" (n), "1" ((long)dest), "2" ((long)src)
		: "memory");
	return dest;
}

void *memmove(void *dest, const void *src, size_t n)
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

void *memscan(void *addr, int c, size_t n)
{
	if (!n)
		return addr;
	__asm__("repnz ; scasb\n\t"
		"jnz 1f\n\t"
		"dec %%edi\n"
		"1:"
		: "=D" (addr), "=c" (n)
		: "0" (addr), "1" (n), "a" (c)
		: "memory");
	return addr;
}

__always_inline void *memset(void *s, char c, size_t n)
{
	int d0, d1;
	__asm__("rep\n\t"
		"stosb"
		: "=&c" (d0), "=&D" (d1)
		: "a" (c), "1" (s), "0" (n)
		: "memory");
	return s;
}
