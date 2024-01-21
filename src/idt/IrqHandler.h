#pragma once

class IRQHandler{
private:
    unsigned short irq_number;
public:
    IRQHandler(unsigned char irq_number) : irq_number(irq_number){}
    virtual void handle_IRQ() = 0;
    unsigned char getIRQNumber() const { return irq_number; }
};