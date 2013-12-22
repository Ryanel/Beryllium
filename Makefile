KERNEL_FILES := $(patsubst %.cpp,%.o,$(wildcard src/*.cpp)) $(patsubst %.c,%.o,$(wildcard src/*.c)) $(patsubst %.s,%.o,$(wildcard src/*.s))
BOOT_FILES := $(patsubst %.s,%.o,$(wildcard boot/*.s))

INCLUDE_DIR := "./src/includes"

C_OPTIONS := -O2
CPP_OPTIONS := -O2
all: boot kernel

boot: ${BOOT_FILES}

kernel: ${KERNEL_FILES}

%.o: %.s
	@echo "Making: " $@
	@nasm -f elf -o $@ $<

%.o: %.c
	@echo "Making: " $@
	@clang -c ${OPTIONS} -I${INCLUDE_DIR} -o $@ $<

%.o: %.cpp
	@echo "Making: " $@
	@clang++ -c ${OPTIONS} -I${INCLUDE_DIR} -o $@ $<

clean: prep-dist
	-rm -rf *.o
	-rm -rf boot/*.o
	-rm -rf src/*.o
prep-dist:
	-rm -rf *~
	-rm -rf boot/*~
	-rm -rf src/*~
