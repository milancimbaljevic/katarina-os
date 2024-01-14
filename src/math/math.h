#pragma once

#include "utils/float.h"

int pow(int number, int power)
{
    if (power == 0)
        return 1;

    int res = number;

    for (int i = 0; i < power - 1; i++)
    {
        res *= number;
    }

    return res;
}

float floor(float value)
{
    FloatParts fp = Float::get_float_parts(value);
    return fp.fractional_part >= 0 ? (float)fp.whole_part : (float)(fp.whole_part - 1);
}

float ceil(float value)
{
    FloatParts fp = Float::get_float_parts(value);
    return fp.fractional_part > 0 ? (float)fp.whole_part + 1 : (float)fp.whole_part;
}

// not realy log 10, more like 10^x where val = v * 10^x
int log10(float value)
{
    if(value < 0) value *= -1;
    int log = 0;
    while (value != 0 && value >= 10)
    {
        value /= 10;
        log++;
    }
    return log;
}
