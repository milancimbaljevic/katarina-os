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
#include "gdt/gdt.h"
#include "config.h"
#include "task/tss.h"
#include "task/task.h"
#include "ds/vector.h"
#include "mbr/mbr_reader.h"

void kernel_panic(string msg)
{
    debugln(string("Kernel panic: ") + msg);
    while (1)
    {
    }
}

class Test
{
public:
int i = 0;
    Test(int i){
        this->i = i;
    }
};

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

struct tss tss;
struct gdt gdt_real[TOTAL_NUMBER_OF_GDT_SEGMENTS];
struct gdt_structured gdt_structured[TOTAL_NUMBER_OF_GDT_SEGMENTS] =
    {
        {.base = 0x00, .limit = 0x00, .type = 0x00},                 // NULL Segment
        {.base = 0x00, .limit = 0xffffffff, .type = 0x9a},           // Kernel code segment
        {.base = 0x00, .limit = 0xffffffff, .type = 0x92},           // Kernel data segment
        {.base = 0x00, .limit = 0xffffffff, .type = 0xf8},           // User code segment
        {.base = 0x00, .limit = 0xffffffff, .type = 0xf2},           // User data segment
        {.base = (uint32_t)&tss, .limit = sizeof(tss), .type = 0xE9} // TSS Segment
};

void kernel_main()
{
    FUNCTION_ENTER(("kernel_main"));

    memset(gdt_real, 0x00, sizeof(gdt_real));
    gdt_structured_to_gdt(gdt_real, gdt_structured, TOTAL_NUMBER_OF_GDT_SEGMENTS);
    gdt_load(gdt_real, sizeof(gdt_real));

    memset(&tss, 0x00, sizeof(tss));
    tss.esp0 = 0x600000;
    tss.ss0 = KERNEL_DATA_SELECTOR;

    // Load the TSS
    tss_load(0x28);

    terminal_initialize();

    ErrorOr<Heap> kheap = Heap::createHeap((void *)0x01000000, (void *)(0x01000000 + 104857600), 4096, (Heap::HEAP_BLOCK_TABLE_ENTRY *)0x00007E00, 104857600 / 4096);
    if (kheap.isError())
    {
        println(kheap.getErrorMsg());
        kernel_panic("Unable to create kernel heap!!!");
    }
    // this is very ugly but it will work for now
    Heap::kernel_heap = &kheap.getRetVal();

    PIC::Init();
    IRQTimer timer_irq_handler(14);
    // PIC::register_irq(&timer_irq_handler);

    char *test = (char *)0x10000;
    test[0] = 'M';
    test[1] = 'i';
    test[2] = 'l';
    test[3] = 'a';
    test[4] = 'n';
    test[5] = '\0';

    println(test);

    PageMapTable *original_pmt = new OneToOnePMT();
    original_pmt->init();
    original_pmt->load_me_into_pmtr();
    enable_paging();

    Disk::init_disks(); // this doesn't do anything for now, that is way i am creating master_disk manually with id 0
    Disk *master_disk = new MasterDisk(0);
    Disk::register_disk(master_disk);

    char *dest = (char *)malloc(master_disk->get_sector_size());
    master_disk->read_sector(0, 1, (void *)dest);

    char *stream_buff = (char *)malloc(50);
    DiskStream *disk_stream = DiskStream::create_disk_stream(0);
    disk_stream->seek(40);
    disk_stream->read(stream_buff, 30);

    original_pmt->map_virtual_to_physical_address((void *)(0x56785000), 0x10000 | PAGING_ACCESS_FROM_ALL | PAGING_IS_PRESENT | PAGING_IS_WRITEABLE);

    char *test1 = (char *)0x56785000;
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


    MBR_Reader::Init();

    sti();
    while (1)
    {
    }
}
