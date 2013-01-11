global start
extern kmain

section .rodata

MODALIGN	equ 1<<0
MEMINFO		equ 1<<1
FLAGS		equ MODALIGN | MEMINFO
MAGIC		equ 0x1BADB002
CHECKSUM	equ -(MAGIC + FLAGS)
STACKSIZ 	equ 0x4000

section .header

align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM
start:
	mov esp, stack + STACKSIZ
	push eax
	push ebx

	call kmain

	cli
.hang:
	hlt
	jmp .hang

section .bss

align 4
stack resb STACKSIZ
