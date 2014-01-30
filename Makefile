BOOT_FILES := boot/boot.o
BOOT_PI_FILES := boot/boot_pi.o

KERNEL_FILES := $(patsubst %.cpp,%.o,$(wildcard src/*.cpp)) $(patsubst %.c,%.o,$(wildcard src/*.c))

LOW_FILES := $(patsubst %.cpp,%.o,$(wildcard src/low/*.cpp)) $(patsubst %.s,%.o,$(wildcard src/low/*.s))

LIB_FILES := $(patsubst %.cpp,%.o,$(wildcard src/lib/*.cpp))

DRIVER_FILES := $(patsubst %.cpp,%.o,$(wildcard src/drivers/*.cpp))

CC:=clang
CPP:=clang++
C_OPTIONS := 
CPP_OPTIONS :=
CLANG_OPTIONS := -ffreestanding -target i586-elf


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

lib: ${LIB_FILES}

drivers: ${DRIVER_FILES}
kernel: boot low lib drivers ${KERNEL_FILES}
	@echo "Linking Kernel"
	@${LD} ${LFLAGS} -T ${LD_SCRIPT} -o kernel.elf ${BOOT_FILES} ${LOW_FILES} ${LIB_FILES} ${KERNEL_FILES} ${DRIVER_FILES}
%.o: %.s
	@echo "Making: " $@
	@${ASM} -o $@ $<

%.o: %.c
	@echo "Making: " $@
	@${CC} -c ${C_OPTIONS} ${CLANG_OPTIONS} ${CROSS_CLANG} -I${INCLUDE_DIR} -o $@ $<

%.o: %.cpp
	@echo "Making: " $@
	@${CPP} -c ${CPP_OPTIONS} ${CLANG_OPTIONS} ${CROSS_CLANG} -I${INCLUDE_DIR} -o $@ $<

clean: prep-dist
	-rm -rf *.od boot/*.o src/*.o src/low/*.o
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

