#pragma once

#include "memory/memory.h"
#include "print.h"

#define DEFAULT_VECTOR_SIZE 100

template <typename T>
struct VectorIterator
{
    VectorIterator(T *ptr) : ptr(ptr) {}
    T &operator*() { return *ptr; }
    VectorIterator &operator++()
    {
        ptr++;
        return *this;
    }
    VectorIterator operator++(int)
    {
        VectorIterator temp = *this;
        ++ptr; // (*this)++
        return temp;
    }
    friend bool operator==(const VectorIterator &it1, const VectorIterator &it2)
    {
        return it1.ptr == it2.ptr;
    }
    friend bool operator!=(const VectorIterator &it1, const VectorIterator &it2)
    {
        return it1.ptr != it2.ptr;
    }

private:
    T *ptr;
};

template <typename T>
class vector
{
    int vector_size;
    T *vector_array = nullptr;
    int free_pointer = 0;

public:
    // this is a very bad implementation
    // i have no way of handling errors
    // for now it will work
    // TODO: find some way to handle error without using C++ error handling mechanism, needed throughout the project

    vector(int size = DEFAULT_VECTOR_SIZE + 1)
    {
        this->vector_size = size;
        vector_array = (T *)malloc(sizeof(T) * this->vector_size) + 1; // i should probably call initialize elements to some defualt value
    }

    void push_back(T t)
    {
        if (vector_size == free_pointer)
        {
            resize();
            push_back(t);
            return;
        }
        vector_array[free_pointer++] = t;
    }

    T pop_back() { return vector_array[free_pointer > 0 ? free_pointer-- : 0]; }

    T front() { return vector_array[0]; }
    T back() { return vector_array[free_pointer >= 1 ? free_pointer - 1 : 0]; }

    T operator[](int pos)
    {
        if (pos >= vector_size)
            pos = vector_size - 1;
        return vector_array[pos];
    }

    int size() { return free_pointer; }
    int capacity() { return vector_size; }

    void resize()
    {
        T *new_array = (T *)malloc(sizeof(T) * this->vector_size * 2) + 1;
        for (int i = 0; i < vector_size; i++)
        {
            new_array[i] = vector_array[i];
        }

        vector_size *= 2;

        free(vector_array);
        vector_array = new_array;
    }

    VectorIterator<T> begin()
    {
        return VectorIterator<T>(&vector_array[0]);
    }

    VectorIterator<T> end()
    {
        return VectorIterator<T>(&vector_array[free_pointer]);
    }

    ~vector()
    {
        free((void *)vector_array);
    }
};