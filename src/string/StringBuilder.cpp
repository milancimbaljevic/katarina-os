#include "StringBuilder.h"
#include <stddef.h>

string StringBuilder::FormatString(string str)
{
    return str;
}

string StringBuilder::to_string(size_t num)
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

string StringBuilder::to_string(unsigned int num)
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

string StringBuilder::to_string(int _num)
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

string StringBuilder::to_string_binary(size_t num)
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

string StringBuilder::to_string_binary(int num)
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

string StringBuilder::to_string_binary(unsigned int num)
{
    return to_string_binary((int)num);
}

string StringBuilder::to_string_binary(char num)
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

string StringBuilder::to_string_binary(unsigned char num)
{
    return to_string_binary((char)num);
}

string StringBuilder::to_string_binary(float num)
{
    ufloat uf;
    uf.f = num;
    return to_string_binary(uf.u);
}

string StringBuilder::to_string_binary(double num)
{
    // FIXME: this is bad
    return to_string_binary((float)num);
}

string StringBuilder::to_string(double value)
{
    // FIXME: this is bad
    return StringBuilder::to_string((float)value);
}

string StringBuilder::to_string(float value)
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

char hex_char[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
string StringBuilder::to_string_hex(unsigned int num)
{
    string res = "";
    while (num != 0)
    {
        unsigned int x = num % 16;
        res = res + hex_char[x];
        num /= 16;
    }
    res = res + "x0";
    return res.reverse();
}

string StringBuilder::to_string_hex(long unsigned int num)
{
    string res = "";
    while (num != 0)
    {
        long unsigned int x = num % 16;
        res = res + hex_char[x];
        num /= 16;
    }
    res = res + "x0";
    return res.reverse();
}


string StringBuilder::to_string_hex(int num)
{
    if(num < 0) num *= -1;
    string res = to_string_hex((unsigned int) num);
    if(num < 0) return string("-") + res;
    return res;
}
