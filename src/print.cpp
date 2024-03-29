#include "print.h"
#include "debug/debugcon.h"

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

void _print(const char *str, size_t len)
{
    size_t i = 0;
    while (i < len)
    {
        terminal_write_char(str[i++], 15);
    }
}

void println(string st)
{
    st = st + '\n';
    _print(st.getCharPointer(), st.size());
}

void print(string st)
{
    _print(st.getCharPointer(), st.size());
}

void print_ram_in_range_inc(unsigned int start, unsigned int end)
{
    for (unsigned int i = start; i <= end; i += 4)
    {
        unsigned int *address = (unsigned int *)i;
        debugln("Address {h} : {h}", i, (unsigned int)(*address));
    }
}

void print_value_at_address(unsigned int addr)
{
    unsigned int *address = (unsigned int *)addr;
    debugln("Address {h} : {h}", addr, (unsigned int)(*address));
}