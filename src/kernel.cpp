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

void kernel_main()
{
    terminal_initialize();

    // string str1 = "Caoooo\nsta se radii";
    // print(str1.getCharPointer(), str1.size());

    ErrorOr<Heap> kernel_heap = Heap::createHeap((void *)0x01000000, (void *)(0x01000000 + 104857600), 4096, (Heap::HEAP_BLOCK_TABLE_ENTRY *)0x00007E00, 104857600 / 4096);

    if (kernel_heap.isError())
    {
        print(kernel_heap.getErrorMsg().getCharPointer(), kernel_heap.getErrorMsg().size());
    }

    // char* str1 = (char*) kernel_heap.getRetVal().malloc(8);
    // str1[0] = 'P'; str1[1] = 'o'; str1[2] = 'z'; str1[3] = 'd'; str1[4] = 'r'; str1[5] = 'a'; str1[6] = 'v'; str1[7] = '\0';

    // char* str2 = (char*) kernel_heap.getRetVal().malloc(8);
    // str2[0] = '\n'; str2[1] = 'o'; str2[2] = 'z'; str2[3] = 'd'; str2[4] = 'r'; str2[5] = 'a'; str2[6] = 'v'; str2[7] = '\0';

    // print(str1, 8);
    // print(str2, 8);

    // string str1 = "Ja sam";
    // string str2 = " Milan";
    // string str3 = str1 + str2;

    // char x = (char)(str3.size() + '0');
    // print(&x, 1);
    // print(str3.substr(7,5).getCharPointer(), 5);
    // print(str3.getCharPointer(), str3.size());

    // char i1 = -12;
    // int i2 = -32;
    // string str4 = StringBuilder::FormatString("Cao {b} ja sam {}", i1, i2);

    float x = 8.5;
    FloatParts fp = Float::get_float_parts(x);

    // unsigned int fpart = (fi.mantissa << 9) << fp.displacment;

    string str5 = StringBuilder::FormatString(
        "sign: {b} \ndisplacment: {} \nwhole_part: {}\nfractional_part : {} \npow_test: {}",
        fp.sign,
        fp.displacment, fp.whole_part, 1, 0.0012);
    print(str5.getCharPointer(), str5.size());

    // print(str4.getCharPointer(), str4.size());
    // print("Marko",5);

    // for(char& c: str3){
    //     print(&c, 1);
    //     print("\n", 1);
    // }

    outb('A', 0xe9);

    while (1)
    {
    }
}
