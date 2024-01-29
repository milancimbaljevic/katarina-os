#pragma once

#include <stddef.h>

void* memset(void* ptr, int c, size_t size);
int memcmp(void* s1, void* s2, int count);
void* memcpy(void* dest, void* src, int len);
void* malloc(size_t size);

void operator delete(void *memory, unsigned long sz);
void *operator new(size_t size);
void operator delete(void *memory);