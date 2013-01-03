CC	= gcc
AS 	= nasm
LD	= ld
CFLAGS	= -Wall -m32 -c -nostdlib -fno-builtin -nostartfiles \
       	  -nodefaultlibs -nostdinc -mno-sse -fno-stack-protector
LDFLAGS	= -Tlink.ld -melf_i386
ASFLAGS	= -felf 

BIN	= bin
BOOT	= boot

TARGET	= $(BIN)/kernel

VPATH	= boot
MKDIR	= $(CURDIR)/$(BIN)

ASSRCS	+= boot.s
SOURCES += kernel.c

OBJS	= $(addprefix $(BIN)/,${SOURCES:.c=.o})
ASOBJS	= $(addprefix $(BIN)/,${ASSRCS:.s=.o})

$(shell `mkdir -p $(MKDIR)`)

all: $(TARGET)

$(TARGET) : $(OBJS) $(ASOBJS)
	$(LD) $(LDFLAGS) -o $(TARGET) $(OBJS) $(ASOBJS)
$(BIN)/%.o : %.c
	$(CC) $(CFLAGS) -o $@ $<
$(BIN)/%.o : %.s
	$(AS) $(ASFLAGS) -o $@ $< 
clean :
	${RM} -r $(BIN)
.PHONY : clean
