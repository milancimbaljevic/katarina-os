#pragma once

#define TOTAL_NUMBER_OF_INTERRUPTS 512
#define KERNEL_CODE_SELECTOR 0x08
#define TOTAL_NUMBER_OF_GDT_SEGMENTS 6
#define KERNEL_DATA_SELECTOR 0x10
#define KERNEL_CODE_SELECTOR 0x08

#define IP_STARTING_VIRTUAL_ADDRESS 0x400000
#define USER_DATA_SEGMENT 0x23
#define USER_CODE_SEGMENT 0x1b
#define VIRTUAL_STACK_ADDRESS_START 0x3FF000
#define USER_PROGRAM_STACK_SIZE 1024 * 16
#define VIRTUAL_STACK_ADDRESS_END VIRTUAL_STACK_ADDRESS_START - USER_PROGRAM_STACK_SIZE
