set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_C_COMPILER clang)
set(CMAKE_ASM_NASM_COMPILER nasm)

set(TARGET_TRIPLET x86_64-elf)
set(COMPILE_FLAGS "-target ${TARGET_TRIPLET} -ffreestanding -mno-red-zone -fno-stack-protector")

set(CMAKE_C_FLAGS_INIT "${COMPILE_FLAGS}")
set(CMAKE_ASM_NASM_FLAGS "-f elf64")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)