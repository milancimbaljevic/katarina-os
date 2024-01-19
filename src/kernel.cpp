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
#include "idt/idt.h"

void kernel_main()
{
    terminal_initialize();

    ErrorOr<Heap> kernel_heap = Heap::createHeap((void *)0x01000000, (void *)(0x01000000 + 104857600), 4096, (Heap::HEAP_BLOCK_TABLE_ENTRY *)0x00007E00, 104857600 / 4096);
    if (kernel_heap.isError())
    {
        println(kernel_heap.getErrorMsg());
    }

    IDT::Init();
    IDT::enable_interrupts();

    while (1)
    {
    }
}
