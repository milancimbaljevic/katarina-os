#pragma once

#include "idt/IrqHandler.h"
#include "print.h"

class IRQTimer : public IRQHandler
{
public:
    IRQTimer(unsigned char irq_number) : IRQHandler(irq_number) {}
    void handle_IRQ() override
    {
        println("IRQTimer irq_number = {}!!!", getIRQNumber());
    }
};