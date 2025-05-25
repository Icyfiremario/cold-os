.PHONY: clean realclean run dump

BUILD_DIR := build/x86_64
DIST_DIR := dist/x86_64
IMPL_DIR := src/impl
INTF_DIR := src/intf

TARGET := $(DIST_DIR)/kernel.iso
ELF := $(BUILD_DIR)/kernel.bin

CC := clang
AS := nasm
LD := ld.lld
COPIER := llvm-objcopy
GRUB := grub-mkrescue

CFLAGS := -g -Wall -O2 -ffreestanding -I $(INTF_DIR) -Wextra -Wpedantic
AFLAGS := -f elf64 -g
LFLAGS := -nostdlib -g
OFLAGS := -O binary
GFLAGS := /usr/lib/grub/i386-pc

X86_64_LINKER := targets/x86_64/linker.ld

KERNEL_SRC := $(wildcard $(IMPL_DIR)/kernel/*.c)
IMPL_HDR := $(wildcard $(INTF_DIR)/*.h)

X86_64_ASM_SRC := $(wildcard $(IMPL_DIR)/x86_64/*.asm) $(wildcard $(IMPL_DIR)/x86_64/boot/*.asm)
X86_64_C_SRC := $(wildcard $(IMPL_DIR)/x86_64/*.c) $(KERNEL_SRC)

X86_64_OBJ := $(X86_64_ASM_SRC:.asm=.o) $(X86_64_C_SRC:.c=.o)

$(TARGET): $(X86_64_OBJ) $(IMPL_HDR)
	mkdir -p $(BUILD_DIR)
	mkdir -p $(DIST_DIR)
	$(LD) $(LFLAGS) -m elf_x86_64 -T $(X86_64_LINKER) $(X86_64_OBJ) -o $(ELF) -Map $(BUILD_DIR)/kernel.map
	llvm-objcopy --only-keep-debug $(ELF) $(BUILD_DIR)/kernel.sym
	cp $(ELF) targets/x86_64/iso/boot/kernel.bin
	$(GRUB) $(GFLAGS) -o $@ targets/x86_64/iso
	

%.o: %.asm
	$(AS) $(AFLAGS) $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) -target x86_64-elf $< -o $@

clean:
	@rm -f $(X86_64_OBJ) $(ELF)
	@rm -f $(BUILD_DIR)/kernel.map
	@rm -f $(BUILD_DIR)/kernel.sym
	@echo Removed object files and elf

realclean: clean
	@rm -f $(TARGET)
	@echo Removed kernel iso

run: $(TARGET)
	qemu-system-x86_64 -S -cdrom dist/x86_64/kernel.iso -s

dump: $(TARGET)
	@clear
	@llvm-objdump -Dx $(ELF)