#pragma once

#include <stddef.h>

#include "string/string.h"

template <typename T>
class ErrorOr
{
private:
    bool is_error = 0;
    string error_msg;
    T return_value;

public:
    ErrorOr(string em)
    {
        error_msg = em;
        is_error = true;
    }
    ErrorOr(T rv)
    {
        return_value = rv;
        is_error = false;
    }

    bool isError()
    {
        return is_error;
    }

    const string &getErrorMsg() const { return error_msg; }

    T &getRetVal() { return return_value; }
};

class Heap
{
public:
    // this is bad becuse each entrie takes two bytes, it could be packed to one byte, but i am lazy
    // FIXME : make it use one byte instead of two
    struct HEAP_BLOCK_TABLE_ENTRY
    {
        bool is_first = false; // 0 - not first, 1 - first
        bool taken = false;    // 0 - free, 1 - taken
    };

private:
    void *start_address = nullptr;
    void *end_address = nullptr;
    size_t block_size = 0; // bytes, usually 4kb

    HEAP_BLOCK_TABLE_ENTRY *entries_table = nullptr;
    size_t total_number_of_table_entries;
    explicit Heap(void *sa, void *ea, size_t bs, HEAP_BLOCK_TABLE_ENTRY *et, size_t tnoe) : start_address(sa), end_address(ea), block_size(bs), entries_table(et), total_number_of_table_entries(tnoe)
    {
        for (size_t i = 0; i < total_number_of_table_entries; i++)
        {
            entries_table[i].is_first = 0;
            entries_table[i].taken = 0;
        }
    }

    static bool validateAlignment(void *addr, size_t block_size);
    static bool validateTable(void *start_addr, void *end_addr, HEAP_BLOCK_TABLE_ENTRY *table, size_t block_size, size_t tnoe);

    size_t alignSizeToUpper(size_t size) const;
public:
    Heap() = default;

    static ErrorOr<Heap> createHeap(void *sa, void *ea, size_t bs, HEAP_BLOCK_TABLE_ENTRY *et, size_t tnoe);

    void *malloc(size_t size_in_bytes);
};