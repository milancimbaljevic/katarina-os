#include "debugcon.h"
#include "print.h"
void debugln(string st)
{
    for (char &c : st)
    {
        outb(c, 0xe9);
    }
    outb('\n', 0xe9);
}

void debug(string st)
{
    for (char &c : st)
    {
        outb(c, 0xe9);
    }
}