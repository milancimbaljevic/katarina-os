#include "memory.h"
#include "heap.h"

void* memset(void* ptr, int c, size_t size){
    char* c_ptr = (char*) ptr;
    for(size_t i=0; i < size; i++){
        c_ptr[i] = (char) c;
    }
    return ptr;
}

int memcmp(void* s1, void* s2, int count)
{
    char* c1 = (char*) s1;
    char* c2 = (char*) s2;
    while(count-- > 0)
    {
        if (*c1++ != *c2++)
        {
            return c1[-1] < c2[-1] ? -1 : 1;
        }
    }

    return 0;
}

void* memcpy(void* dest, void* src, int len)
{
    char *d = (char*) dest;
    char *s = (char*) src;
    while(len--)
    {
        *d++ = *s++;
    }
    return dest;
}

void* malloc(size_t size){
    return Heap::kernel_heap->malloc(size);
}

void free(void* addr){
    Heap::kernel_heap->free(addr);
}


void *operator new(size_t size)
{
    return malloc(size);
}

void operator delete(void *memory)
{
    free(memory);
}

void operator delete(void *memory, unsigned long sz)
{
    free(memory);
}