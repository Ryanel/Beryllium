BOOT_FILES := boot/boot.s
BOOT_PI_FILES := boot/boot_pi.s

KERNEL_FILES := $(patsubst %.cpp,%.o,$(wildcard src/*.cpp)) $(patsubst %.c,%.o,$(wildcard src/*.c))

LOW_FILES := $(patsubst %.cpp,%.o,$(wildcard src/low/*.cpp)) $(patsubst %.s,%.o,$(wildcard src/low/*.s))

LIB_FILES := $(patsubst %.cpp,%.o,$(wildcard src/lib/*.cpp))

CC:=clang
CPP:=clang++
C_OPTIONS := 
CPP_OPTIONS := 
CLANG_OPTIONS := -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -target i586-elf

LD := ./toolkit/binutils/bin/i586-elf-ld
LFLAGS := -m elf_i386
LD_SCRIPT := src/link.ld
INCLUDE_DIR := "./src/includes"

ARCH := x86
CROSS_CLANG := -target i586-elf
ASM := nasm -f elfb

all:clean boot kernel

boot: ${BOOT_FILES}

low: ${LOW_FILES}

lib: ${LIB_FILES}

kernel: boot low lib ${KERNEL_FILES}
	@echo "Linking Kernel"
	@${LD} ${LFLAGS} -T ${LD_SCRIPT} -o kernel.elf ${BOOT_FILES} ${LOW_FILES} ${LIB_FILES} ${KERNEL_FILES}
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
	qemu -kernel kernel.elf
