C_SRC := $(shell find . -name "*.c") 

C_OBJ := $(patsubst %.c, %.o, $(C_SRC))

INCLUDES := -I ./include
INCLUDES += -I ./include/mem
INCLUDES += -I ./include/hw 
INCLUDES += -I ./include/device
INCLUDES += -I ./include/trap
INCLUDES += -I ./include/thread 

CC := riscv64-unknown-elf-gcc
CASM := riscv64-unknown-elf-as

CFLAG := -c -g2 -O0 \
			-nostdlib -nostartfiles -nodefaultlibs \
			-fno-builtin -fno-exceptions \
			-march=rv64i -mcmodel=medany

OUTPUT_DIR = ./obj
BIN_DIR = ./bin
ELF_FILE = testbench.o
BIN_FILE = testbench.bin

.PHONY: build link qemu run clean

%.o:%.c
	$(CC) $(CFLAG) $(INCLUDES) $< -o $@

%.o:%.S
	$(CC) $(CFLAG)  $< -o $@

build: $(ASM_OBJ) $(C_OBJ)

copy:
	riscv64-unknown-elf-ld -T linker.ld $(C_OBJ) -o $(ELF_FILE)
	riscv64-unknown-elf-objcopy $(ELF_FILE) --strip-all -O binary $(BIN_FILE)
	cp $(ELF_FILE) ../RiscvOsInC/src/thread/	

run: build copy

clean:
	rm ./src/main/*.o
	rm ./src/utilities/*.o
	rm ./src/device/*.o

	rm *bin
	rm *elf
	


