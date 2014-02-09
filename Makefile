COMPILE_OPTIONS := -D DEBUG

BOOT_FILES := boot/boot.o
BOOT_PI_FILES := boot/boot_pi.o

KERNEL_FILES := $(patsubst %.c,%.o,$(wildcard src/*.c))

LOW_FILES := $(patsubst %.c,%.o,$(wildcard src/low/*.c))

LIB_FILES := $(patsubst %.c,%.o,$(wildcard src/lib/*.c))

DRIVER_FILES := $(patsubst %.c,%.o,$(wildcard src/drivers/*.c))

X86_FILES := $(patsubst %.c,%.o,$(wildcard src/x86/*.c)) $(patsubst %.s,%.o,$(wildcard src/x86/*.s))

CC:=clang
CPP:=clang++
C_OPTIONS := -std=gnu99 -ffreestanding -O2 -Wall -Wextra
CPP_OPTIONS := 
CLANG_OPTIONS := -target i586-elf

LD := ./toolkit/binutils/bin/i586-elf-ld
LFLAGS := -m elf_i386
LD_SCRIPT := src/link.ld
INCLUDE_DIR := "./src/includes"

ARCH := x86
CROSS_CLANG := -target i586-elf
ASM := nasm -f elf 

GENISO := xorriso -as mkisofs

.PHONY: iso clean

all:clean boot kernel drivers iso

boot: ${BOOT_FILES}

low: ${LOW_FILES}

x86f: ${X86_FILES}

lib: ${LIB_FILES}

drivers: ${DRIVER_FILES}
kernel: boot low lib drivers x86f ${KERNEL_FILES}
	@echo "Linking Kernel"
	@${LD} ${LFLAGS} -T ${LD_SCRIPT} -o kernel.elf ${BOOT_FILES} ${X86_FILES} ${LOW_FILES} ${LIB_FILES} ${KERNEL_FILES} ${DRIVER_FILES}
%.o: %.s
	@echo "Making: " $@
	@${ASM} -o $@ $<

%.o: %.c
	@echo "Making: " $@
	@${CC} -c ${C_OPTIONS} ${CLANG_OPTIONS} ${CROSS_CLANG} ${COMPILE_OPTIONS} -I${INCLUDE_DIR} -o $@ $<

%.o: %.cpp
	@echo "Making: " $@
	@${CPP} -c ${CPP_OPTIONS} ${CLANG_OPTIONS} ${CROSS_CLANG} ${COMPILE_OPTIONS} -I${INCLUDE_DIR} -o $@ $<

clean: prep-dist
	-rm -rf *.o boot/*.o src/*.o src/low/*.o src/lib/*.o src/drivers/*.o src/x86/*.o
	-rm -rf kernel.elf
prep-dist:
	-rm -rf *~ boot/*~ src/*~
run:
	@echo "Remember! Use make run to test the kernel! Implement it into a OS if you wish to test other fuctions!"
	qemu-system-i386 -serial stdio -cdrom cdrom.iso

iso:
	@echo "Creating ISO..."
	cp kernel.elf iso/kernel.elf
	@${GENISO} -R -b boot/grub/stage2_eltorito -quiet -no-emul-boot -boot-load-size 4 -boot-info-table -o cdrom.iso iso

