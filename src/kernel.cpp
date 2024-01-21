#include <stdint.h>
#include <stddef.h>

#include "kernel.h"
#include "string/string.h"
#include "memory/heap.h"
#include "io/sys_io.h"
#include "string/StringBuilder.h"
#include "utils/float.h"
#include "math/math.h"
#include "print.h"
#include "debug/debugcon.h"
#include "idt/pic.h"
#include "timer/IRQTimer.h"

void kernel_panic()
{
    debugln("Kernel panic!!!");
    while (1)
    {
    }
}

void kernel_main()
{
    terminal_initialize();

    ErrorOr<Heap> kheap = Heap::createHeap((void *)0x01000000, (void *)(0x01000000 + 104857600), 4096, (Heap::HEAP_BLOCK_TABLE_ENTRY *)0x00007E00, 104857600 / 4096);
    if (kheap.isError())
    {
        println(kheap.getErrorMsg());
        kernel_panic();
    }
    // this is very ugly but it will work for now
    Heap::kernel_heap = &kheap.getRetVal();

    PIC::Init();
    IRQTimer timer_irq_handler(14);
    PIC::register_irq(&timer_irq_handler);

    sti();
    while (1)
    {
    }
}
