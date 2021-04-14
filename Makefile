CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump

CFLAGS=-mfpu=neon-vfpv4 -march=armv7-a -mtune=cortex-a7 -DRPI2=1 -O4 -g -nostartfiles -mfloat-abi=hard -Iusb/uspi/ -Iusb/

SOURCE=
HEADER=
ASM=

ASM+=startup.S
SOURCE+=kernel.c
SOURCE+=cstartup.c
SOURCE+=gic-400.c
HEADER+=gic-400.h
SOURCE+=timer.c
HEADER+=timer.h
SOURCE+=uart.c
HEADER+=uart.h
HEADER+=base.h
SOURCE+=gpio.c
HEADER+=gpio.h
SOURCE+=interrupts.c
HEADER+=interrupts.h
SOURCE+=mailbox.c
HEADER+=mailbox.h
SOURCE+=framebuffer.c
HEADER+=framebuffer.h
SOURCE+=graphics.c
HEADER+=graphics.h
SOURCE+=font_regular.c
HEADER+=font_regular.h
SOURCE+=board.c
HEADER+=board.h

OBJS=$(join $(addprefix obj/, $(dir $(SOURCE))), $(notdir $(SOURCE:.c=.o)))
OBJS+=$(join $(addprefix obj/, $(dir $(ASM))), $(notdir $(ASM:.S=.o)))

IMG=kernel7.img
EXE=kernel7.elf
BINDIR=bin
OBJDIR=obj
DISASM=kernel7.asm

obj/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

obj/%.o: %.S
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: all
all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BINDIR)/$(EXE)
	$(OBJCOPY) $(BINDIR)/$(EXE) -O binary $(BINDIR)/$(IMG)
	$(OBJDUMP) -l -S -D $(BINDIR)/$(EXE) > $(BINDIR)/$(DISASM)

.PHONY: clean
clean:
	rm -rf $(BINDIR)/$(EXE)
	rm -rf $(BINDIR)/$(IMG)
	rm -rf $(OBJDIR)/*.o
