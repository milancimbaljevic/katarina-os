#pragma once

struct FloatInfo
{
    unsigned char sign = 0;
    unsigned char exponent = 0;
    unsigned int mantissa = 0;
};

struct FloatParts
{
    unsigned char sign;
    int whole_part;
    float fractional_part;
    char displacment; // normal exponent
};

union ufloat
{
    float f;
    unsigned int u;
};

class Float
{
public:
    static FloatInfo get_float_info(float _num);
    static FloatParts get_float_parts(float value);
};
