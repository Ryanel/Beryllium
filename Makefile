COMPILE_OPTIONS := -D DEBUG -D ENABLE_SERIAL

BOOT_FILES := boot/boot.o

KERNEL_FILES := $(patsubst %.c,%.o,$(wildcard src/*.c))

LOW_FILES := $(patsubst %.c,%.o,$(wildcard src/low/*.c))

LIB_FILES := $(patsubst %.c,%.o,$(wildcard src/lib/*.c))

DRIVER_FILES := $(patsubst %.c,%.o,$(wildcard src/drivers/*.c))

X86_FILES := $(patsubst %.c,%.o,$(wildcard src/x86/*.c)) $(patsubst %.s,%.o,$(wildcard src/x86/*.s))

ARM_FILES := $(patsubst %.c,%.o,$(wildcard src/arm/*.c)) $(patsubst %.s,%.o,$(wildcard src/arm/*.s))

CC:=clang -DX86 -target i586-elf
CPP:=clang++
C_OPTIONS := -std=gnu99 -ffreestanding -Wall 
CPP_OPTIONS := 
CLANG_OPTIONS := 
ARMTK:=./toolkit/arm-2008q3/bin/arm-none-eabi

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
#=== x86 ====
boot: ${BOOT_FILES}

low: ${LOW_FILES}

x86f: ${X86_FILES}

lib: ${LIB_FILES}

drivers: ${DRIVER_FILES}

kernel: boot low lib drivers x86f ${KERNEL_FILES}
	@echo "Linking Kernel"
	@${LD} ${LFLAGS} -T ${LD_SCRIPT} -o kernel.elf ${BOOT_FILES} ${X86_FILES} ${LOW_FILES} ${LIB_FILES} ${KERNEL_FILES} ${DRIVER_FILES}

#ARM
boot-arm: ${BOOT_FILES}

arm-files: ${ARM_FILES}

arm:
	make clean kernel-arm rpi-image BOOT_FILES=boot/boot_pi.o CC="${ARMTK}-gcc -DARM" ASM=${ARMTK}-as LD=${ARMTK}-ld LFLAGS="" LD_SCRIPT=src/arm/rpi.ld

kernel-arm: boot-arm low lib arm-files ${KERNEL_FILES}
	@echo "Linking Kernel"
	@${LD} ${LFLAGS} -T ${LD_SCRIPT} -o kernel.elf ${BOOT_FILES} ${ARM_FILES} ${LOW_FILES} ${LIB_FILES} ${KERNEL_FILES}

rpi-image: kernel.elf
	$(ARMTK)-objcopy kernel.elf -O binary kernel.img
#Generic

%.o: %.s
	@echo "Making: " $@
	@${ASM} -o $@ $<

%.o: %.c
	@echo "Making: " $@
	@${CC} -c ${C_OPTIONS} ${COMPILE_OPTIONS} -I${INCLUDE_DIR} -o $@ $<

%.o: %.cpp
	@echo "Making: " $@
	@${CPP} -c ${CPP_OPTIONS}  ${COMPILE_OPTIONS} -I${INCLUDE_DIR} -o $@ $<

clean: prep-dist
	-rm -rf *.o boot/*.o src/*.o src/low/*.o src/lib/*.o src/drivers/*.o src/x86/*.o
	-rm -rf util/*.o util/*.bin
	-rm -rf *.iso
	-rm -rf kernel.elf kernel.img
prep-dist:
	-rm -rf *~ boot/*~ src/*~
run:
	@echo "Remember! Use make run to test the kernel! Implement it into a OS if you wish to test other fuctions!"
	qemu-system-i386 -serial stdio -cdrom cdrom.iso

iso:
	@echo "Creating ISO..."
	@cp kernel.elf iso/kernel.elf
	@${GENISO} -R -b boot/grub/stage2_eltorito -quiet -no-emul-boot -boot-load-size 4 -boot-info-table -o cdrom.iso iso

util: util-iboot-iso
	@echo "Built Utilities"

util/iboot.bin:
	@nasm util/iboot-iso.s -f bin -o util/iboot.bin

util-iboot: util/iboot.bin
	@cp util/iboot.bin iso/boot/iboot.bin
	@echo "Integrated Bootloader Built"

util-iboot-iso: util-iboot
	@echo "Creating iboot ISO..."
	@cp kernel.elf iso/kernel.elf
	@${GENISO} -R -J -c boot/bootcat -b boot/iboot.bin -no-emul-boot -boot-info-table -boot-load-size 4 iso -o iboot.iso