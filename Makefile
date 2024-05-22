.PHONY: clean realclean run

TARGET := kernel.iso
ELF := kernel.bin

BUILD_DIR := build
IMPL_DIR := src/impl
INTF_DIR := src/intf

DIST_DIR := dist/x86_64

CC := clang
AS := nasm
LD := ld.lld
COPIER := llvm-objcopy
GRUB := 

CFLAGS := -Wall -O2 -ffreestanding -nostdinc -nostdlib -I $(INTF_DIR)
AFLAGS := -f elf64
LFLAGS := -nostdlib
OFLAGS := -O binary

X86_64_LINKER := targets/x86_64/linker.ld

KERNEL_SRC := $(wildcard $(IMPL_DIR)/kernel/*.c)
IMPL_HDR := $(wildcard $(INTF_DIR)/*.h)

X86_64_BOOT_SRC := $(wildcard $(IMPL_DIR)/x86_64/boot/*.asm)
X86_64_IMPL_SRC := $(wildcard $(IMPL_DIR)/x86_64/*.c) $(KERNEL_SRC)

X86_64_OBJ := $(X86_64_BOOT_SRC:.asm=.o) $(X86_64_IMPL_SRC:.c=.o)

$(TARGET): $(X86_64_OBJ)
	mkdir -p $(BUILD_DIR)
	mkdir -p $(DIST_DIR)
	$(LD) $(LFLAGS) -T $(X86_64_LINKER) $(X86_64_OBJ) -o $(BUILD_DIR)/$(ELF)
	$(COPIER) $(OFLAGS) $(BUILD_DIR)/$(ELF) $(DIST_DIR)/$@
	

%.o: %.asm
	$(AS) $(AFLAGS) $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) -target x86_64-elf $< -o $@

clean:
	@rm -f $(X86_64_OBJ) $(BUILD_DIR)/$(ELF)
	@echo Removed object files and elf

realclean: clean
	@rm -f $(DIST_DIR)/$(TARGET)
	@echo Removed kernel iso

run:
	qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso