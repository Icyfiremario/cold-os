cmake_minimum_required(VERSION 3.13)

project(CMAKE_OS_TEST LANGUAGES C ASM_NASM)

set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/targets/x86_64/linker.ld")
set(ISO_STAGING_DIR "${CMAKE_SOURCE_DIR}/targets/x86_64/iso")
set(DIST_DIR "${CMAKE_SOURCE_DIR}/dist")

file(GLOB_RECURSE SOURCES
    "src/impl/*.c"
    "src/impl/*.asm"
)

add_executable(kernel.bin ${SOURCES})

target_include_directories(kernel.bin PRIVATE src/intf)

target_compile_options(kernel.bin PRIVATE
    -g -Wall -O2 -Wextra -Wpedantic
)

target_link_options(kernel.bin PRIVATE
    "-nostdlib"
    "-fuse-ld=lld"
    "-static"
    "-T${LINKER_SCRIPT}"
    "-Wl,-Map=${CMAKE_BINARY_DIR}/kernel.map"
)

set(KERNEL_ISO "${DIST_DIR}/kernel.iso")

add_custom_command(TARGET kernel.bin POST_BUILD
    COMMAND llvm-objcopy --only-keep-debug $<TARGET_FILE:kernel.bin> ${CMAKE_BINARY_DIR}/kernel.sym
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:kernel.bin> ${ISO_STAGING_DIR}/boot/kernel.bin
    DEPENDS SOURCES
    COMMENT "Staging kernel for ISO creation"
)

add_custom_target(iso ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DIST_DIR}
    COMMAND grub-mkrescue -d /usr/lib/grub/i386-pc -o ${KERNEL_ISO} ${ISO_STAGING_DIR}
    DEPENDS kernel.bin
    COMMENT "Building bootable ISO"
)

add_custom_target(run
    COMMAND qemu-system-x86_64 -cdrom ${KERNEL_ISO} -s
    DEPENDS iso
)