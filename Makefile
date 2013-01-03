CC	= gcc
CFLAGS	= -Wall -m32 -nostdlib -fno-builtin -nostartfiles \
       	  -nodefaultlibs -nostdinc -mno-sse -fno-stack-protector
LDFLAGS	= -T link.ld -m elf_i386
ASFLAGS	= -felf 

BOOT	= boot

TARGET	= kernel

SOURCES += $(BOOT)/boot.o
SOURCES += $(BOOT)/kernel.o

all: $(SOURCES) link

clean:
	${RM} $(BOOT)/*.o kernel
link:
	ld $(LDFLAGS) -o $(TARGET) $(SOURCES)
.s.o:
	nasm $(ASFLAGS) $< 
