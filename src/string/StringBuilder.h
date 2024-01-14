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

    static string to_string(unsigned int num)
    {
        if (num == 0)
            return '0';

        string res;
        while (num != 0)
        {
            unsigned int c = num % 10;
            num /= 10;
            char ch = (char)(c + '0');
            res = res + string(ch);
        }
        return res.reverse();
    }

    static string to_string(int _num)
    {
        if (_num == 0)
            return '0';

        int num = _num > 0 ? _num : -_num;

        string res;
        while (num != 0)
        {
            unsigned int c = num % 10;
            num /= 10;
            char ch = (char)(c + '0');
            res = res + string(ch);
        }
        if (_num < 0)
            res = res + string('-');
        return res.reverse();
    }

    static string to_string_hex(int num)
    {
        return "";
    }

    static string to_string_binary(int num)
    {
        string res = "32b";
        for (int i = 0; i < 32; i++)
        {
            if (((unsigned int)0x80000000 >> i) & num)
                res = res + string('1');
            else
                res = res + string('0');
        }

        return res;
        // string res_without_lead_zeros = "32b";

        // int j = 0;
        // while (j < 32 && res[j++] == '0')
        //     ;

        // if (j == 32)
        //     return "32b0";

        // j--;

        // while (j < 32)
        // {
        //     res_without_lead_zeros = res_without_lead_zeros + res[j++];
        // }

        // return res_without_lead_zeros;
    }

    static string to_string_binary(unsigned int num)
    {
        return to_string_binary((int)num);
    }

    static string to_string_binary(char num)
    {
        string res = "8b";
        for (int i = 0; i < 8; i++)
        {
            if (((unsigned int)0b10000000 >> i) & num)
                res = res + string('1');
            else
                res = res + string('0');
        }

        return res;
        // string res_without_lead_zeros = "8b";

        // int j = 0;
        // while (j < 8 && res[j++] == '0')
        //     ;

        // if (j == 8)
        //     return "8b0";

        // j--;

        // while (j < 8)
        // {
        //     res_without_lead_zeros = res_without_lead_zeros + res[j++];
        // }

        // return res_without_lead_zeros;
    }

    static string to_string_binary(unsigned char num)
    {
        return to_string_binary((char)num);
    }

    static string to_string_binary(float num)
    {
        ufloat uf;
        uf.f = num;
        return to_string_binary(uf.u);
    }

    static string to_string_binary(double num)
    {
        // FIXME: this is bad
        return to_string_binary((float)num);
    }

    static string to_string(double value)
    {
        // FIXME: this is bad
        return to_string((float)value);
    }

    static string to_string(float value)
    {
        const int c_maxDigits = 6;

        string decimalDigits = "";
        int numDigits = 0;
        int exponent = 0;

        if (value < 0)
        {
            decimalDigits = decimalDigits + '-';
            value *= -1;
        }

        exponent = log10(value);

        // Scale the input value such that the first digit is in the ones place
        // (e.g. 122.5 would become 1.225).
        value = value / (float)pow(10, exponent);

        // while there is a non-zero value to print and we have room in the buffer
        while (value > 0.0 && numDigits < c_maxDigits)
        {
            if (numDigits == exponent + 1)
                decimalDigits = decimalDigits + '.';
            // Output the current digit.
            float digit = floor(value);
            decimalDigits = decimalDigits + string('0' + (char)digit); // convert to an ASCII character
            ++numDigits;

            // Compute the remainder by subtracting the current digit
            // (e.g. 1.225 would becom 0.225)
            value -= digit;

            // Scale the next digit into the ones place.
            // (e.g. 0.225 would becom 2.25)
            value *= 10.0;
        }

        return decimalDigits;
    }
};