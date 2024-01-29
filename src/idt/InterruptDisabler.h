#pragma once

#define cli() asm("cli")
#define sti() asm("sti")

static inline unsigned int cpuFlags()
{
    unsigned int flags;
    asm volatile(
        "pushf\n"
        "pop %0\n"
        :"=rm"(flags)
        ::"memory");
    return flags;
}

inline bool are_interrupts_enabled()
{
    return cpuFlags() & 0x200;
}

class InterruptDisabler {
public:
    InterruptDisabler()
    {
        m_flags = cpuFlags();
        cli();
    }

    ~InterruptDisabler()
    {
        if (m_flags & 0x200)
            sti();
    }

private:
    unsigned int m_flags;
};