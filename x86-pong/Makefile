PROJECT = pong

SOURCES += src/main.c

LIBPATH = lib
include $(LIBPATH)/lib.mk

CC = gcc
AS = nasm
LD = ld
CP = cp
DEL = rm
QEMU = qemu-system-i386

CFLAGS  += -nostdinc -nostdlib -nodefaultlibs -nostartfiles -static \
	   -ffreestanding -fno-builtin -fno-exceptions -fno-stack-protector \
	   -m32 -std=gnu99 -O0 -Wall -Wextra -g
ASFLAGS = -f elf32 -g -F dwarf -O0
LDFLAGS = -m elf_i386 -nostdlib

OBJECTS = $(SOURCES:.c=.o)

all: $(PROJECT).elf

$(PROJECT).iso: $(PROJECT).elf
	$(CP) $< iso/boot/kernel.elf
	grub-mkrescue -o $@ iso

$(PROJECT).elf: boot/loader.o boot/isr.o $(OBJECTS)
	$(LD) $(LDFLAGS) -T linker.ld -o $@ $^

%.o: %.S
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $(CCFLAGS) $<

clean:
	$(DEL) -f **/*.elf *.o **/*.o **/**/*.o iso/boot/*.elf *.img *.iso *.elf

run: $(PROJECT).iso
	$(QEMU) -soundhw pcspk -monitor stdio -hda $<

debug: kernel.elf
	$(QEMU) -soundhw pcspk -serial mon:stdio -s -S -kernel $< &
	gdb -iex "set auto-load safe-path .gdbinit" $<

