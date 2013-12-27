KERNEL_FILES := $(patsubst %.cpp,%.o,$(wildcard src/*.cpp)) $(patsubst %.c,%.o,$(wildcard src/*.c)) $(patsubst %.s,%.o,$(wildcard src/*.s))
LOW_FILES := $(patsubst %.cpp,%.o,$(wildcard src/low/*.cpp)) $(patsubst %.s,%.o,$(wildcard src/low/*.s))
BOOT_FILES := $(patsubst %.s,%.o,$(wildcard boot/*.s))

LD := ./toolkit/binutils/bin/i586-elf-ld
LFLAGS := -m elf_i386
LD_SCRIPT := src/link.ld
INCLUDE_DIR := "./src/includes"

C_OPTIONS := -O2 
CPP_OPTIONS := -O2
CLANG_OPTIONS := -target i586-elf
all:clean boot kernel

boot: ${BOOT_FILES}

low: ${LOW_FILES}

kernel: boot low ${KERNEL_FILES}
	@echo "Linking Kernel"
	@${LD} ${LFLAGS} -T ${LD_SCRIPT} -o kernel.elf ${BOOT_FILES} ${LOW_FILES} ${KERNEL_FILES}
%.o: %.s
	@echo "Making: " $@
	@nasm -f elf -o $@ $<

%.o: %.c
	@echo "Making: " $@
	@clang -c ${OPTIONS} ${CLANG_OPTIONS} -I${INCLUDE_DIR} -o $@ $<

%.o: %.cpp
	@echo "Making: " $@
	@clang++ -c ${OPTIONS} ${CLANG_OPTIONS} -I${INCLUDE_DIR} -o $@ $<

clean: prep-dist
	-rm -rf *.o boot/*.o src/*.o src/low/*.o
	-rm -rf kernel.elf
prep-dist:
	-rm -rf *~ boot/*~ src/*~
run:
	@echo "Remember! Use make run to test the kernel! Implement it into a OS if you wish to test other fuctions!"
	qemu -kernel kernel.elf
