#pragma once

#include <stdint.h>
#include <stddef.h>
#include "string/string.h"
#include "string/StringBuilder.h"

#define VGA_WIDTH 80
#define VGA_HEGHT 20 

uint16_t terminal_make_char(char c, char color);
void terminal_put_char(int x, int y, char c, char color);
void terminal_write_char(char c, char color);
void terminal_initialize();
void _print(const char *st, size_t len);

template<typename T, typename... Args>
void print(string str, T t, Args... args){
    string st = StringBuilder::FormatString(str, t, args...);
    _print(st.getCharPointer(), st.size());
}


template<typename T, typename... Args>
void println(string str, T t, Args... args){
    string st = StringBuilder::FormatString(str, t, args...);
    st = st + '\n';
    _print(st.getCharPointer(), st.size());
}

void println(string st);
void print(string st);

void print_ram_in_range_inc(unsigned int start, unsigned int end);
void print_value_at_address(unsigned int addr);