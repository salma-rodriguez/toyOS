;
; kernel start location; defines multiboot header.
; based on Bran's kernel development tutorial (start.asm)
;

MBOOT_PAGE_ALIGN	equ 1<<0	; load kern & mod on a page
					; boundary
MBOOT_MEM_INFO		equ 1<<1	; provide your kernel with
					; memory info
MBOOT_HEADER_MAGIC	equ 0x1BADB002	; Multiboot Magic value
MBOOT_HEADER_FLAGS	equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM		equ - (MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

[BITS 32]				; All instructions are
					; 32-bit instructions.
[GLOBAL mboot]				; Make 'mboot' accessible from C.
[EXTERN code]				; Start of the '.text' region.
[EXTERN bss]				; Start of the '.bss' region.
[EXTERN end]				; End of the last loadable region.

mboot:
	dd MBOOT_HEADER_MAGIC		; GRUB will search for this value
					; on each 4-byte boundary in file
	dd MBOOT_HEADER_FLAGS		; How GRUB loads files & settings
	dd MBOOT_CHECKSUM		; ensure above values are correct

	dd mboot			; location of this discriptor
	dd code				; start of kern '.text' section.
	dd bss				; end of kernel '.data' section.
	dd end				; end of kernel
	dd start			; kernel entry point (init EIP).

[GLOBAL start]				; kernel entry point.
[EXTERN main]				; entry point of C code

start:
	push		ebx		; load multiboot header location
	; execute the kernel:
	cli				; Disable interrupts.
	call main			; Call the main() function.
	jmp $				; infinite loop to stop processor
					; execute whatever is in memory
					; after the kernel
