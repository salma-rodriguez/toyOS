; My first bootloader. Doesn't do much.

[BITS 16]		; tell assembler that its a 16 bit code
[ORG 0x7C00]		; Origin, tell the assembler where in memory
			; the code will be after it is loaded
MOV SI, HelloString 
CALL PrintString
JMP $			; infinite loop

PrintChar: 		; procedure to print character on screen
			; Assume that ASCII value is in register AL
			; Now, fill the remaining registers.
MOV AH, 0x0E		; Tell BIOS that we need to print one character on screen.
MOV BH, 0X00		; Page number
MOV BL, 0x07		; lightgrey font on dark background

INT 0x10		; Call video interrupt
RET			;Return to calling procedure

PrintString:		; procedure to print string on screen
			; Assume that string starting pointer is
			; in regjister SI

next_char:		; label to fetch next character from string
MOV AL, [SI]		; Get a byte from string and store in AL register.
INC SI			; Increment SI pointer
OR AL, AL		; Check if value in AL is zero (end of string).
JZ exit_function	; if end, return
CALL PrintChar		; else, print the character in AL register
JMP next_char		; fetch next character from string
exit_function:		; end label
MOV AL, 10		; just a line feed
CALL PrintChar
MOV AL, 13		; carriage return
CALL PrintChar		; last call to printchar
RET			; return from procedure

; data
HelloString db 'Hello World', 0 ; HelloWorld string ending with lf

TIMES 510 - ($ - $$) db 0	; fill remainder of sector with 0
DW 0xAA55			; add boot signature
