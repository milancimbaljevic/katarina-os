#pragma once

#include "string.h"
#include "math/math.h"

class StringBuilder
{
public:
    // FIXME : dummy solution, just like rest of the codebase :)
    static string FormatString(string str);
    template <typename T, typename... Args>
    static string FormatString(string str, T param, Args... args);
    static string to_string(unsigned int num);
    static string to_string(int _num);
    static string to_string_hex(int num);
    static string to_string_binary(int num);
    static string to_string_binary(unsigned int num);
    static string to_string_binary(char num);
    static string to_string_binary(unsigned char num);
    static string to_string_binary(float num);
    static string to_string_binary(double num);
    static string to_string(double value);
    static string to_string(float value);
};