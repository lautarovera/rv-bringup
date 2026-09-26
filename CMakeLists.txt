cmake_minimum_required(VERSION 3.21)

project(rv-bringup LANGUAGES C ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(QEMU_ARGS -machine virt -m 128M -smp 1 -nographic -bios none -device edu)

add_executable(
    rv-bringup.elf
    src/main.c
    src/uart.c
    src/start.S
    src/trap_entry.S
    src/trap.c
    src/timer.c
    src/smode.c
    src/plic.c
    src/pcie.c
    src/edu.c
)

set_target_properties(rv-bringup.elf PROPERTIES LINK_DEPENDS ${CMAKE_SOURCE_DIR}/link.ld)

target_include_directories(rv-bringup.elf PRIVATE include)

target_compile_options(rv-bringup.elf PRIVATE -mcmodel=medany -ffreestanding -Wall -Wextra -g -Og)

target_link_options(rv-bringup.elf PRIVATE -nostdlib -T${CMAKE_SOURCE_DIR}/link.ld -Wl,--no-warn-rwx-segments -Wl,-Map=${CMAKE_BINARY_DIR}/rv-bringup.map)

add_custom_target(run COMMAND qemu-system-riscv64 ${QEMU_ARGS} -kernel "$<TARGET_FILE:rv-bringup.elf>" USES_TERMINAL VERBATIM)

add_custom_target(debug COMMAND qemu-system-riscv64 ${QEMU_ARGS} -kernel "$<TARGET_FILE:rv-bringup.elf>" -s -S USES_TERMINAL VERBATIM)

enable_testing()

add_test(NAME boot COMMAND ${CMAKE_SOURCE_DIR}/tests/run_qemu.sh $<TARGET_FILE:rv-bringup.elf>)
