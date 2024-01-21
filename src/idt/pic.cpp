#include "pic.h"
#include "memory/memory.h"
#include "io/sys_io.h"
#include "print.h"
#include "debug/debugcon.h"

#define SLAVE_INDEX 2
#define IRQ_VECTOR_BASE 0x50

namespace PIC
{
    IRQHandler *irq_handlers[16] = {nullptr};

    void eoi(unsigned char irq)
    {
        if (irq & 8)
            outb(PIC1_CTL, 0x20);
        outb(PIC0_CTL, 0x20);
    }

    struct idt_desc idt_table[TOTAL_NUMBER_OF_INTERRUPTS];
    struct idtr_desc idt_register;

    void int21h_handler()
    {
        cli();
        pushad();

        println("Keyboard pressed!");
        outb(0x20, 0x20);

        popad();
        sti();
    }

    void handle_irq()
    {
        cli();
        pushad();

        unsigned short isr = PIC::getISR();
        if (!isr)
        {
            debugln("Spurious IRQ\n");
            return;
        }

        unsigned char irq = 0;
        for (unsigned char i = 0; i < 16; ++i)
        {
            if (isr & (1 << i))
            {
                irq = i;
                break;
            }
        }
        
        if (irq_handlers[irq])
            irq_handlers[irq]->handle_IRQ();

        eoi(irq);
        popad();
        sti();
    }

    unsigned short getISR()
    {
        outb(PIC0_CTL, 0x0b);
        outb(PIC1_CTL, 0x0b);
        unsigned char isr0 = inb(PIC0_CTL);
        unsigned char isr1 = inb(PIC1_CTL);
        return (isr1 << 8) | isr0;
    }

    void timer_handler()
    {
        cli();
        pushad();

        unsigned short isr = PIC::getISR();
        if (!isr)
        {
            debugln("Spurious IRQ\n");
            sti();
            return;
        }

        unsigned char irq = 0;
        for (unsigned char i = 0; i < 16; ++i)
        {
            if (isr & (1 << i))
            {
                irq = i;
                break;
            }
        }
        println("Cao iz tajmera {}", irq);

        eoi(0);
        popad();
        sti();
    }

    void no_interrupt()
    {
        cli();
        outb(0x20, 0x20);
        sti();
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
        
        for(int i=0x20; i<0x20+16; i++){
            idt_set(i, (void *)handle_irq);
        }

        idt_load(&idt_register);
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

    void disable(unsigned char irq)
    {
        unsigned char imr;
        if (irq & 8)
        {
            imr = inb(PIC1_CMD);
            imr |= 1 << (irq - 8);
            outb(PIC1_CMD, imr);
        }
        else
        {
            imr = inb(PIC0_CMD);
            imr |= 1 << irq;
            outb(PIC0_CMD, imr);
        }
    }

    void enable(unsigned char irq)
    {
        unsigned char imr;
        if (irq & 8)
        {
            imr = inb(PIC1_CMD);
            imr &= ~(1 << (irq - 8));
            outb(PIC1_CMD, imr);
        }
        else
        {
            imr = inb(PIC0_CMD);
            imr &= ~(1 << irq);
            outb(PIC0_CMD, imr);
        }
    }

    void register_irq(IRQHandler *irq_handler)
    {
        irq_handlers[irq_handler->getIRQNumber()] = irq_handler;
    }

    void initialize()
    {
        /* ICW1 (edge triggered mode, cascading controllers, expect ICW4) */
        outb(PIC0_CTL, 0x11);
        outb(PIC1_CTL, 0x11);

        /* ICW2 (upper 5 bits specify ISR indices, lower 3 idunno) */
        outb(PIC0_CMD, IRQ_VECTOR_BASE);
        outb(PIC1_CMD, IRQ_VECTOR_BASE + 0x08);

        /* ICW3 (configure master/slave relationship) */
        outb(PIC0_CMD, 1 << SLAVE_INDEX);
        outb(PIC1_CMD, SLAVE_INDEX);

        /* ICW4 (set x86 mode) */
        outb(PIC0_CMD, 0x01);
        outb(PIC1_CMD, 0x01);

        // Mask -- start out with all IRQs disabled.
        // outb(PIC0_CMD, 0xff);
        // outb(PIC1_CMD, 0xff);

        outb(PIC0_CMD, 0x00);
        outb(PIC1_CMD, 0x00);

        // ...except IRQ2, since that's needed for the master to let through slave interrupts.
        enable(2);
    }

};