CC	= gcc
AS 	= nasm
LD	= ld
BUILD	= kernel
CFLAGS	= -Wall -m32 -c -I./include -nostdlib -mno-sse \
	  -nostartfiles -nodefaultlibs -nostdinc -fno-builtin \
	  -fno-stack-protector
LDFLAGS	= -Tlink.ld -melf_i386
ASFLAGS	= -felf 

BIN	= bin
TARGET	= $(BIN)/$(BUILD)

VPATH	= boot lib init kernel
MKDIR	= $(CURDIR)/$(BIN)

# under boot
ASSRCS	+= boot.s
# under kernel
ASSRCS  += interrupt.s segment.s
# under lib
SOURCES += string.c ctype.c printf.c
# under init
SOURCES += common.c monitor.c kernel.c
# under kernel
SOURCES += isr.c tables.c

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
