#pragma once

#include "string.h"
#include "math/math.h"

class StringBuilder
{
public:
    template <typename T, typename... Args>
    static string FormatString(string str, T param, Args... args)
    {
        string res = "";
        int i = 0;
        while (true)
        {
            if (str[i] == '{' && str[i > 0 ? i - 1 : i] != '\\')
            {
                // TODO: Add cases for {, {b, {h
                if (i + 1 >= str.size())
                    return "FORMAT '{' ERROR";

                if (str[i + 1] == 'h')
                {
                    i++;
                    res = res + to_string_hex(param);
                }
                else if (str[i + 1] == 'b')
                {
                    i++;
                    res = res + to_string_binary(param);
                }
                else
                    res = res + to_string(param);

                i += 2;

                if constexpr (sizeof...(args) == 0)
                {
                    while (i < str.size())
                    {
                        res = res + str[i++];
                    }
                    return res;
                }
                else
                {
                    return res + FormatString(str.substr(i, str.size() - i), args...);
                }

                break;
            }
            res = res + str[i++];
        }
    }

    // FIXME : dummy solution, just like rest of the codebase :)
    static string FormatString(string str);
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