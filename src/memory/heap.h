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
    ErrorOr(string em){
        error_msg = em;
        is_error = true;
    }
    ErrorOr(T rv){
        return_value = rv;
        is_error = false;
    }

    bool isError() {
        return is_error;
    }

    const string& getErrorMsg() const { return error_msg; }

    T& retVal() { return return_value; }

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
    explicit Heap(void *sa, void *ea, size_t bs, HEAP_BLOCK_TABLE_ENTRY *et, size_t tnoe) : start_address(sa), end_address(ea), block_size(bs), entries_table(et), total_number_of_table_entries(tnoe) {}

    static bool validateAlignment(void* addr, size_t block_size){
        return ((unsigned int)addr % block_size) == 0;
    }

    static bool validateTable(void* start_addr, void* end_addr, HEAP_BLOCK_TABLE_ENTRY* table, size_t block_size, size_t tnoe){
        size_t heap_size = (size_t) end_addr - (size_t) start_addr;
        size_t total_blocks_in_heap = heap_size / block_size;

        return tnoe == total_blocks_in_heap;
    }

public:
    Heap() = default;

    static ErrorOr<Heap> createHeap(void *sa, void *ea, size_t bs, HEAP_BLOCK_TABLE_ENTRY* et, size_t tnoe)
    {
        // Validate start_addres, end_addres and block_size
        if (!validateAlignment(sa, bs) || !validateAlignment(ea, bs))
        {
            return ErrorOr<Heap>("Error in createHeap(void*, void*, size_t): addresses not aligned");
        }

        if(!validateTable(sa, ea, et, bs, tnoe)){
            return ErrorOr<Heap>("Error in createHeap(void*, void*, size_t): entries table is not valid");
        }
        

        return ErrorOr<Heap>(Heap(sa, ea, bs, et, tnoe));
    }
};