// #pragma once

// #include <stddef.h>

// template <typename T>
// class ErrorOr
// {
// private:
//     char is_error = 0;
//     char error_msg[20];
//     T return_value;
// public:
//     ErrorOr(const char* em){
//         error_msg = em;
//     }
// };

// class Heap
// {
// private:
//     void *start_address = nullptr;
//     void *end_address = nullptr;
//     size_t block_size = 0; // bytes
//     struct HEAP_BLOCK_TABLE_ENTRY
//     {
//         char is_first = 0; // 0 - not first, 1 - first
//         char taken = 0;    // 0 - free, 1 - taken
//         HEAP_BLOCK_TABLE_ENTRY *next_entrie = nullptr;
//     };
//     HEAP_BLOCK_TABLE_ENTRY *entries_table = nullptr;
//     explicit Heap(void *sa, void *ea, size_t bs, HEAP_BLOCK_TABLE_ENTRY *et) : start_address(sa), end_address(ea), block_size(bs), entries_table(et) {}

// public:
//     static ErrorOr<Heap *> createHeap(void *sa, void *ea, size_t bs)
//     {
//         // Validate start_addres, end_addres and block_size
//         if (!validate_alignment(sa) || !validate_alignment(ea))
//         {
//         }
//     }
// };