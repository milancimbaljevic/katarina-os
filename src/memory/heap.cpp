#include "heap.h"
#include "debug/debugcon.h"
#include "kernel.h"
#include "debug/debug.h"

Heap *Heap::kernel_heap = nullptr;

bool Heap::validateAlignment(void *addr, size_t block_size)
{
    return ((unsigned int)addr % block_size) == 0;
}

bool Heap::validateTable(void *start_addr, void *end_addr, HEAP_BLOCK_TABLE_ENTRY *table, size_t block_size, size_t tnoe)
{
    size_t heap_size = (size_t)end_addr - (size_t)start_addr;
    size_t total_blocks_in_heap = heap_size / block_size;

    return tnoe == total_blocks_in_heap;
}

size_t Heap::alignSizeToUpper(size_t size) const
{
    if (size % block_size == 0)
        return size;
    return (size - (size % block_size) + block_size);
}

ErrorOr<Heap> Heap::createHeap(void *sa, void *ea, size_t bs, HEAP_BLOCK_TABLE_ENTRY *et, size_t tnoe)
{
    // Validate start_addres, end_addres and block_size
    if (!validateAlignment(sa, bs) || !validateAlignment(ea, bs))
    {
        return ErrorOr<Heap>("Error in createHeap(void*, void*, size_t): addresses not aligned");
    }

    if (!validateTable(sa, ea, et, bs, tnoe))
    {
        return ErrorOr<Heap>("Error in createHeap(void*, void*, size_t): entries table is not valid");
    }

    return ErrorOr<Heap>(Heap(sa, ea, bs, et, tnoe));
}

void *Heap::malloc(size_t size_in_bytes)
{
    FUNCTION_ENTER(("Heap::malloc"));

    size_in_bytes = alignSizeToUpper(size_in_bytes);

    // convert size in bytes to size in blocks
    size_t size_in_blocks = size_in_bytes / block_size;

    if (size_in_blocks > total_number_of_table_entries)
        return nullptr;

    // search for size_in_block continuous free blocks

    int start_block = -1;
    size_t block_counter = 0;

    for (size_t i = 0; i < total_number_of_table_entries; i++)
    {

        if (entries_table[i].taken)
        {
            start_block = -1;
            block_counter = 0;
            continue;
        }

        if (start_block == -1)
            start_block = i;
        block_counter++;

        if (block_counter == size_in_blocks)
            break;
    }

    if (start_block == -1 || block_counter != size_in_blocks)
        return nullptr;

    void *return_address = (void *)((size_t)start_address + (size_t)start_block * block_size);

    // mark blocks as taken
    entries_table[start_block].is_first = true;
    size_t i;
    for (i = start_block; i < start_block + size_in_blocks; i++)
    {
        entries_table[i].taken = true;
        entries_table[i].has_next = true;
    }
    entries_table[i - 1].has_next = false;

    DEBUGLN(("Allocated {} bytes of memory starting at {h}", size_in_blocks * block_size, (unsigned int) return_address));

    return return_address;
}

void Heap::free(void *mem)
{
    FUNCTION_ENTER(("HEAP::free"));
    
    size_t heap_block_start = ((int)mem - (int)start_address) / block_size;
    entries_table[heap_block_start].is_first = false;

    int size_in_block = 0;
    for (size_t i = heap_block_start; i < total_number_of_table_entries; i++)
    {
        if(entries_table[i].is_first) break;

        size_in_block++;
        entries_table[i].taken = false;
        
        if (!entries_table[i].has_next)
        {
            break;
        }
        entries_table[i].has_next = false;
    }

    DEBUGLN(("Freed {} bytes of memory starting at {h}", size_in_block * block_size, (unsigned int)mem));
}

// FIXME: i have no idea why is this causing undefined reference issues
void *__gxx_personality_v0 = 0;
void *_Unwind_Resume = 0;