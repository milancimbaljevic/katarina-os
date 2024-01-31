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
#include "paging/paging.h"
#include "disk/Disk.h"
#include "disk/DiskStream.h"

void kernel_panic()
{
    debugln("Kernel panic!!!");
    while (1)
    {
    }
}

// class NewOperatorTest
// {
// public:
//     int arr[16];
//     string name;
//     NewOperatorTest(string name) : name(name) {}

//     void *operator new(size_t size)
//     {
//         return Heap::kernel_heap->malloc(size);
//     }

//     void operator delete(void *memory)
//     {
//         Heap::kernel_heap->free(memory);
//     }
// };


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
    IRQTimer timer_irq_handler(14);
    //PIC::register_irq(&timer_irq_handler);


    char* test = (char*) 0x10000;
    test[0] = 'M'; test[1] = 'i'; test[2] = 'l'; test[3] = 'a'; test[4] = 'n'; test[5] = '\0';
    
    println(test);
    
    PageMapTable *original_pmt = new OneToOnePMT();
    original_pmt->init();
    original_pmt->load_me_into_pmtr();
    enable_paging();

    Disk::init_disks(); // this doesn't do anything for now, that is way i am creating master_disk manually with id 0
    Disk* master_disk = new MasterDisk(0);
    Disk::register_disk(master_disk);

    char* dest = (char*) malloc(master_disk->get_sector_size());
    master_disk->read_sector(0, 1, (void*) dest);

    char* stream_buff = (char*) malloc(50);
    DiskStream* disk_stream = DiskStream::create_disk_stream(0);  
    disk_stream->seek(40);  
    disk_stream->read(stream_buff, 30);

    original_pmt->map_virtual_to_physical_address((void*) (0x56785000), 0x10000 | PAGING_ACCESS_FROM_ALL | PAGING_IS_PRESENT | PAGING_IS_WRITEABLE);

    char* test1 = (char*) 0x56785000;
    println(string(test1));

    // NewOperatorTest* t1 = new NewOperatorTest("Milan");
    // delete t1;

    // NewOperatorTest* t2 = new NewOperatorTest("t2");
    // NewOperatorTest* t3 = new NewOperatorTest("t3");
    // delete t2;
    // NewOperatorTest* t4 = new NewOperatorTest("t4");
    // delete t4;
    // delete t3;

    // print_ram_in_range_inc(0x7c00, 0x7c00 + 515);
    // print_value_at_address(0x100000);

    sti();
    while (1)
    {
    }
}
