CC	= gcc
AS 	= nasm
LD	= ld
BUILD	= kernel.bin
CFLAGS	= -Wall -m32 -c -I./include -nostdlib -mno-sse \
	  -nostartfiles -nodefaultlibs -nostdinc -fno-builtin \
	  -fno-stack-protector
LDFLAGS	= -Tlink.ld -zmax-page-size=0x1000 -melf_i386
ASFLAGS	= -felf 

BIN	= bin
TARGET	= $(BIN)/$(BUILD)

VPATH	= boot lib init kernel mm fs sched
MKDIR	= $(CURDIR)/$(BIN)

define CMP
	@echo " [CC] $@" && $(CC)
endef

define ASM
	@echo " [AS] $@" && $(AS)
endef

define LDD
	@echo " [LD] $@" && $(LD)
endef

define REM
	@echo " [RM] $(MKDIR)" && ${RM}
endef

# under boot
ASSRCS	+= boot.s
# under kernel
ASSRCS  += interrupt.s flush.s
# under sched
ASSRCS  += process.s
# under lib
SOURCES += string.c ctype.c printf.c array.c ordered_map.c
# under init
SOURCES += monitor.c kernel.c initrd.c
# under kernel
SOURCES += isr.c tables.c timer.c common.c panic.c bug.c handler.c
# under mm
SOURCES += page.c heap.c clone.c
# under fs
SOURCES += fs.c

OBJS	= $(addprefix $(BIN)/,${SOURCES:.c=.o})
ASOBJS	= $(addprefix $(BIN)/,${ASSRCS:.s=.o})

$(shell `mkdir -p $(MKDIR)`)

all: $(TARGET)

$(TARGET) : $(OBJS) $(ASOBJS)
	$(LDD) $(LDFLAGS) -o $(TARGET) $(OBJS) $(ASOBJS)
$(BIN)/%.o : %.c
	$(CMP) $(CFLAGS) -o $@ $<
$(BIN)/%.o : %.s
	$(ASM) $(ASFLAGS) -o $@ $< 
clean :
	$(REM) -r $(BIN)
.PHONY : clean
