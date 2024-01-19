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
void print(string str, T t, Args... args);

template<typename T, typename... Args>
void println(string str, T t, Args... args);

void println(string st);
void print(string st);