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
#include "debug/debug.h"

void kernel_panic()
{
    debugln("Kernel panic!!!");
    while (1)
    {
    }
}

class NewOperatorTest{
public:
    int arr[16];
    string name;
    NewOperatorTest(string name) : name(name) {}

    void* operator new(size_t size){
        return Heap::kernel_heap->malloc(size);
    }

    void operator delete(void* memory){
        Heap::kernel_heap->free(memory);
    }
};

void kernel_main()
{
    FUNCTION_ENTER(("kernel_main"));
    
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
    // IRQTimer timer_irq_handler(14);
    // PIC::register_irq(&timer_irq_handler);

    NewOperatorTest* t1 = new NewOperatorTest("Milan");
    delete t1;

    NewOperatorTest* t2 = new NewOperatorTest("t2");
    NewOperatorTest* t3 = new NewOperatorTest("t3");
    delete t2;
    NewOperatorTest* t4 = new NewOperatorTest("t4");
    delete t4;
    delete t3;

    sti();
    while (1)
    {
    }
}