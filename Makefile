# C_SRC := $(shell find . -name "*.c") 
# ASM_SRC := $(shell find . -name "*.S")

# C_OBJ := $(patsubst %.c, %.o, $(C_SRC))
# ASM_OBJ := $(patsubst %.S, %.o, $(ASM_SRC))

C_SRC := ./src/main/*.c \
		 ./src/trap/*.c \
		 ./src/utilities/*.c \
		 ./src/device/*.c \
		 ./src/mm/ipimem.c

ASM_SRC := ./src/asm/trap.S

EXCLUDE_FILES := lock.c,handler.c,print.c,interrupt.c,math.c,rb.c,reg.c,trap.S,entry.c

INCLUDES := ./include/
# INCLUDES += -I ./include/mem/
# INCLUDES += -I ./include/hw/ 
# INCLUDES += -I ./include/device/
# INCLUDES += -I ./include/trap/
# INCLUDES += -I ./include/thread/ 

CC := riscv64-unknown-elf-gcc
CASM := riscv64-unknown-elf-as

CFLAG := -g2 -O0 \
			-nostdlib -nostartfiles -nodefaultlibs \
			-fno-builtin -fno-exceptions \
			-march=rv64imac -mcmodel=medany \
			-finstrument-functions \

OUTPUT_DIR = ./obj
BIN_DIR = ./bin
ELF_FILE = testbench.elf
BIN_FILE = testbench.bin

.PHONY: build clean

# %.o:%.c
# 	$(CC) $(CFLAG) $(INCLUDES) $< -o $@ -finstrument-functions-exclude-file-list=$(EXCLUDE_FILES)

# %.o:%.S
# 	$(CC) $(CFLAG)  $< -o $@

# build: $(ASM_OBJ) $(C_OBJ)
# OS_LIB_2 = $(OS_DIR_2)/src_mt/*.c -I $(OS_DIR_2)/include -T $(OS_DIR_2)/linker.ld -finstrument-functions-exclude-file-list=$(OS_SRC_FILES_2)

build:
	$(CC) $(C_SRC) $(ASM_SRC) -I $(INCLUDES) -T linker.ld -finstrument-functions-exclude-file-list=$(EXCLUDE_FILES) $(CFLAG) -o $(ELF_FILE)
	cp $(ELF_FILE) ../RiscvOsInC/src/thread/	

run: build

clean:
	rm ./src/main/*.o
	rm ./src/utilities/*.o
	rm ./src/device/*.o

	rm *elf
	


# -finstrument-functions-exclude-file-list=$(EXCLUDE_FILES)

# Input Parameters
# ENABLE_PROFILING := 0
# MES_VERSION := 10_3
# LOG_LEVEL := INFO
# #name of output elf file
# ifndef OUTPUT
# 	$(error OUTPUT is not set)
# endif
# INSTRUMENT := 0 # enable time trace of all functions
# MULTICORE := 0 # enable multicore mode

# #
# # Support libraries
# #


# OS_DIR = ./os_lib
# OS_SRC_FILES = rs64irq.c,math.c,os.c,printf.c,profile.c,sbi.c,trap.s,log.c	   
# OS_LIB = $(OS_DIR)/src/*.c -I $(OS_DIR)/include -T $(OS_DIR)/linker.ld -finstrument-functions-exclude-file-list=$(OS_SRC_FILES)

# TRAP_ASM := $(OS_DIR)/src/trap.s

# CP_EMU_DIR = ./cp_emu
# CP_EMU_LIB = $(CP_EMU_DIR)/src/*.c -I $(CP_EMU_DIR)/include

# CP_EMU_DIR_NXT = ./cp_emu_nxt
# CP_EMU_LIB_NXT = $(CP_EMU_DIR_NXT)/src/*.cpp -I $(CP_EMU_DIR_NXT)/include

# OS_DIR_2 = ./os_lib
# RS64_SRC = $(OS_DIR_2)/src_mt/rs64irq.cpp
# OS_SRC_FILES_2 = rs64irq.cpp,math.c,os.c,printf.c,profile.c,sbi.c,trap.s,log.c,./cp_emu_nxt/src/cp_emu_nxt.cpp
# OS_LIB_2 = $(OS_DIR_2)/src_mt/*.c -I $(OS_DIR_2)/include -T $(OS_DIR_2)/linker.ld -finstrument-functions-exclude-file-list=$(OS_SRC_FILES_2)



# #
# # MES compilation files
# #

# MES_INCLUDE              := ../inc
# MES_INCLUDE_HEADERS      := ./mes_support_files/$(MES_VERSION)
# MES_INCLUDE_ASIC_HEADERS := ../../src/inc/mes_$(MES_VERSION)_ip_headers
# MES_INTERNAL_HEADERS              := ../

# MES_CSRC   := ../mes_core_sch.cpp \
# 			  ../mes_hqd_mgr.cpp \
# 			  ../mes_external_if.cpp \
# 			  ../mes_api_processor.cpp \
# 			  ../mes_list_mgr.cpp	\
# 			  ../mes_vmid_mgr.cpp	\
# 			  ../mes_sch_log.cpp \
# 			  ../mes_queue_reset.cpp

# #
# # Compiler configuration
# #

# RISCVCOMPL := riscv64-unknown-elf-gcc
# CFLAG := -march=rv64imafdc -O0 -nostartfiles -nostdlib -nodefaultlibs -fno-builtin -fno-exceptions -g2 -D LOG_LEVEL=LOG_LEVEL_$(LOG_LEVEL)

# FINSTRUMENT := -finstrument-functions -D PROFILE_ENABLE
# KERNEL_MODE := -D KERNEL_MODE $(TRAP_ASM)

# ifeq ($(INSTRUMENT),1)
# 	CFLAG += $(FINSTRUMENT)
# endif

# ifeq ($(MULTICORE),1)
# 	CFLAG += $(KERNEL_MODE)
# endif


# OUTPUT_FOLDER := ./build_output
# OUTPUT_ELF := $(OUTPUT_FOLDER)/$(OUTPUT)

# #
# # Targets
# #

# .PHONY: thread1 thread2 mes oversubscription mtc

# T2SRC := ./tests/test.c
# thread2:
# 	$(RISCVCOMPL) \
# 		$(OS_LIB) $(CP_EMU_LIB) \
# 		$(T2SRC) \
# 		$(CFLAG) -o $(OUTPUT_ELF)


# MTSRC := ./tests/mtc.c

# mtc:
# 	$(RISCVCOMPL) \
# 		-I $(MES_INCLUDE) -I $(MES_INCLUDE_ASIC_HEADERS) -I $(MES_INCLUDE_HEADERS) -I $(MES_INTERNAL_HEADERS)\
# 		$(OS_LIB) $(CP_EMU_LIB) \
# 		$(MTSRC) \
# 		$(KERNEL_MODE) \
# 		$(CFLAG) -o $(OUTPUT_ELF)
		
# MTSSRC := ./tests/mtc_submit.c

# mtcs:
# 	$(RISCVCOMPL) \
# 		-I $(MES_INCLUDE) -I $(MES_INCLUDE_ASIC_HEADERS) -I $(MES_INCLUDE_HEADERS) -I $(MES_INTERNAL_HEADERS)\
# 		$(OS_LIB) $(CP_EMU_LIB) \
# 		$(MTSSRC) \
# 		$(KERNEL_MODE) \
# 		$(CFLAG) -o $(OUTPUT_ELF)


# mtmes:
# 	$(RISCVCOMPL) \
# 		-I $(MES_INCLUDE) -I $(MES_INCLUDE_ASIC_HEADERS) -I $(MES_INCLUDE_HEADERS) -I $(MES_INTERNAL_HEADERS)\
# 		$(OS_LIB_2) $(CP_EMU_LIB_NXT) \
# 		$(MES_CSRC) \
# 		$(RS64_SRC) \
# 		$(KERNEL_MODE) \
# 		$(CFLAG) -o ./build_output/mtmes.elf

# #
# # Deprecated targets (will not use once multicore enabled)
# #

# ifneq ($(MULTICORE),1)

# CP_EMU_OLD_DIR := ./cp_emu_old
# CP_EMU_OLD_INCLUDE := -I $(CP_EMU_OLD_DIR)/include
# CP_EMU_OLD_SRC_FILES := hw_queue.cpp,rs64_mes_api.cpp
# CP_EMU_OLD_LIB := $(CP_EMU_OLD_DIR)/src/* $(CP_EMU_OLD_INCLUDE) -finstrument-functions-exclude-file-list=$(CP_EMU_OLD_SRC_FILES)

# T1SRC := ./tests/cmd.c
# thread1:
# 	$(RISCVCOMPL) $(OS_LIB) \
# 		$(T1SRC) \
# 		$(CFLAG) -o $(OUTPUT_ELF)

# hello_world:
# 	$(RISCVCOMPL) $(OS_LIB) \
# 		./tests/hello_world.c \
# 		$(CFLAG) -o $(OUTPUT_ELF)

# mes:
# 	$(RISCVCOMPL) \
# 		-I $(MES_INCLUDE) -I $(MES_INCLUDE_ASIC_HEADERS) -I $(MES_INCLUDE_HEADERS) -I $(MES_INTERNAL_HEADERS) \
# 		$(OS_LIB) $(CP_EMU_OLD_LIB) \
# 		$(MES_CSRC) \
# 		$(CFLAG) -o $(OUTPUT_ELF)

# oversubscription:
# 	$(RISCVCOMPL) \
# 		-I $(MES_INCLUDE) -I $(MES_INCLUDE_ASIC_HEADERS) -I $(MES_INCLUDE_HEADERS) -I $(MES_INTERNAL_HEADERS) \
# 		$(OS_LIB) $(CP_EMU_OLD_INCLUDE) \
# 		./tests/oversubscription.cpp \
# 		$(CFLAG) -o $(OUTPUT_ELF)

# daoyuan_ticket:
# 	$(RISCVCOMPL) \
# 		-I $(MES_INCLUDE) -I $(MES_INCLUDE_ASIC_HEADERS) -I $(MES_INCLUDE_HEADERS) -I $(MES_INTERNAL_HEADERS) \
# 		$(OS_LIB) $(CP_EMU_OLD_INCLUDE) \
# 		./tests/daoyuan_ticket.cpp \
# 		$(CFLAG) -o $(OUTPUT_ELF)
# endif
