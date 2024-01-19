#include "idt.h"
#include "memory/memory.h"
#include "io/sys_io.h"
#include "print.h"

namespace IDT
{

    struct idt_desc idt_table[TOTAL_NUMBER_OF_INTERRUPTS];
    struct idtr_desc idt_register;

    extern "C" void int21h_handler()
    {
        println("Keyboard pressed!");
        outb(0x20, 0x20);
    }

    void Init()
    {
        memset(&idt_table, 0, sizeof(idt_table));
        idt_register.limit = sizeof(idt_table) - 1;
        idt_register.base = (uint32_t)idt_table;
        for (int i = 0; i < TOTAL_NUMBER_OF_INTERRUPTS; i++)
        {
            idt_set(i, (void *)no_interrupt);
        }

        idt_set(0x20, (void *)int21h);

        idt_load(&idt_register);
    }

    extern "C" void no_interrupt_handler()
    {
        outb(0x20, 0x20);
    }

    void set_irq_handler(const IRQHandler &handler)
    {
    }

    void idt_set(int interrupt_no, void *address)
    {
        struct idt_desc *desc = &idt_table[interrupt_no];
        desc->offset_1 = (uint32_t)address & 0x0000ffff;
        desc->selector = KERNEL_CODE_SELECTOR;
        desc->zero = 0x00;
        desc->type_attr = 0xEE;
        desc->offset_2 = (uint32_t)address >> 16;
    }

};