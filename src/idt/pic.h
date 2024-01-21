#pragma once

#include <stdint.h>
#include "IrqHandler.h"
#include "config.h"

#define pushad() asm("pushal")
#define popad() asm("popal")
#define cli() asm("cli")
#define sti() asm("sti")

#define PIC0_CTL 0x20
#define PIC0_CMD 0x21
#define PIC1_CTL 0xA0
#define PIC1_CMD 0xA1

#define IRQ_VECTOR_OFFSET 0x20

namespace PIC
{
    extern "C" void idt_load(struct idtr_desc *ptr);
    void eoi(unsigned char irq);
    void register_irq(IRQHandler *irq_handler);
    unsigned short getISR();
    void initialize();
    void enable(unsigned char irq);

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