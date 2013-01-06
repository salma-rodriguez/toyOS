/*
 * String and array functions
 * Based on Linus's original code
 * No changes here... yet, but for the record...
 * I will try to move the ints back to register ints
 * Maybe not; code doesn't need to be that optimized!
 * Anyway, these functions will be useful for later.
 * Next: add memcpy, memset, memmove functions, etc.
 */

#include <string.h>

inline int strlen(const char * s)
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

inline int strcmp(const char * cs, const char * ct)
{
	int roslyn, roxana;
	int res;
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

inline char * strcat(char *dest, const char *src)
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

inline char * strcpy(char * dest, const char * src)
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
