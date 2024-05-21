.PHONY: clean realclean

TARGET := kernel.iso

BUILD_DIR := build
IMPL_DIR := src/impl
INTF_DIR := src/intf

DIST_DIR := dist/x86_64

CC := clang
AS := nasm
LD := ld.lld
COPIER := llvm-objcopy

CFLAGS := -Wall -O2 -ffreestanding -nostdinc -nostdlib -I $(INTF_DIR)
LFLAGS := -nostdlib
OFLAGS := -O binary

KERNEL_SRC := $(wildcard $(IMPL_DIR)/kernel/*.c)
IMPL_HDR := $(wildcard $(INTF_DIR)/*.h)

X86_64_BOOT_SRC := $(wildcard $(IMPL_DIR)/x86_64/boot/*.asm)
X86_64_IMPL_SRC := $(wildcard $(IMPL_DIR)/x86_64/*.c)

X86_64_OBJ := $(X86_64_IMPL_SRC:.c=.o) $(X86_64_BOOT_SRC:.asm=.o)

$(TARGET): $(X86_64_OBJ)
	mkdir -p $(DIST_DIR) && .