#pragma once

#include <stdint.h>
#include "IrqHandler.h"
#include "config.h"

namespace IDT
{
    extern "C" void idt_load(struct idtr_desc *ptr);
    extern "C" void enable_interrupts();
    extern "C" void disable_interrupts();
    extern "C" void int21h();
    extern "C" void no_interrupt();
    extern "C" void disable_interrupts();


    struct idt_desc
    {
        uint16_t offset_1;
        uint16_t selector;
        uint8_t zero;
        uint8_t type_attr;
        uint16_t offset_2;
    } __attribute__((packed));

    struct idtr_desc
    {
        uint16_t limit; // size of descriptor table -1
        uint32_t base;  // base address of the start of interrupt descriptor table
    } __attribute__((packed));

    void Init();
    void set_irq_handler(const IRQHandler &handler);
    void idt_set(int interrupt_no, void *address);
};