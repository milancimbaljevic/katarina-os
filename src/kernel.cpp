#include <stdint.h>
#include <stddef.h>

#include "kernel.h"
#include "string/string.h"
#include "memory/heap.h"

uint16_t *video_mem = 0;
int terminal_col = 0;
int terminal_row = 0;

uint16_t terminal_make_char(char c, char color)
{
    return (color << 8) | c;
}

void terminal_put_char(int x, int y, char c, char color)
{
    video_mem[y * VGA_WIDTH + x] = terminal_make_char(c, color);
}

void terminal_write_char(char c, char color)
{
    if (c == '\n')
    {
        terminal_row++;
        terminal_col = 0;
        return;
    }

    terminal_put_char(terminal_col, terminal_row, c, color);
    terminal_col++;
    if (terminal_col >= VGA_WIDTH)
    {
        terminal_col = 0;
        terminal_row += 1;
    }
}

void terminal_initialize()
{
    video_mem = (uint16_t *)(0xB8000);
    for (int y = 0; y < VGA_HEGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            terminal_put_char(x, y, ' ', 0);
        }
    }
}

void print(const char *str, size_t len)
{
    size_t i = 0;
    while (i < len)
    {
        terminal_write_char(str[i++], 15);
    }
}

void kernel_main()
{
    terminal_initialize();

    // string str1 = "Caoooo\nsta se radii";
    // print(str1.getCharPointer(), str1.size());

    ErrorOr<Heap> kernel_heap = Heap::createHeap((void*) 0x01000000, (void*) (0x01000000 + 104857600), 4096, (Heap::HEAP_BLOCK_TABLE_ENTRY*) 0x00007E00, 104857600/4096);

    if(kernel_heap.isError()){
        print(kernel_heap.getErrorMsg().getCharPointer(), kernel_heap.getErrorMsg().size());
    }

    char* str1 = (char*) kernel_heap.getRetVal().malloc(8);
    str1[0] = 'P'; str1[1] = 'o'; str1[2] = 'z'; str1[3] = 'd'; str1[4] = 'r'; str1[5] = 'a'; str1[6] = 'v'; str1[7] = '\0';
    
    char* str2 = (char*) kernel_heap.getRetVal().malloc(8);
    str2[0] = '\n'; str2[1] = 'o'; str2[2] = 'z'; str2[3] = 'd'; str2[4] = 'r'; str2[5] = 'a'; str2[6] = 'v'; str2[7] = '\0';

    print(str1, 8);
    print(str2, 8);

    while(1) {}
}




