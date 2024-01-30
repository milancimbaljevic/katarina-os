#include "Disk.h"
#include "io/sys_io.h"

Disk *Disk::disks[Disk::max_number_of_disks] = {nullptr};

int MasterDisk::read_sector(unsigned long logical_block_address, unsigned long total_number_of_blocks_to_read, void *destination_to_write_in)
{
    outb((logical_block_address >> 24) | 0xE0, 0x1F6);
    outb(total_number_of_blocks_to_read, 0x1F2);
    outb((unsigned char)(logical_block_address & 0xff), 0x1F3);
    outb((unsigned char)(logical_block_address >> 8), 0x1F4);
    outb((unsigned char)(logical_block_address >> 16), 0x1F5);
    outb(0x20, 0x1F7);

    unsigned short *ptr = (unsigned short *)destination_to_write_in;
    for (unsigned long b = 0; b < total_number_of_blocks_to_read; b++)
    {
        // Wait for the buffer to be ready
        char c = inb(0x1F7);
        while (!(c & 0x08))
        {
            c = inb(0x1F7);
        }

        // Copy from hard disk to memory
        for (int i = 0; i < 256; i++)
        {
            *ptr = inw(0x1F0);
            ptr++;
        }
    }
    return 0;
}

void Disk::init_disks()
{
    // there should be some way to scan for disks in the system ad register them
    // for now i will register the MASTER disk manually
    // what am i even talking about, this is all very stupid :)
}