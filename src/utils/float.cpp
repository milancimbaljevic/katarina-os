#include "float.h"

FloatInfo Float::get_float_info(float _num)
{
    ufloat u1;
    u1.f = _num;
    unsigned int num = u1.u;

    FloatInfo fi;
    fi.sign = (unsigned char)((unsigned int)num >> 31);
    fi.exponent = (unsigned char)(((unsigned int)num << 1) >> 24);
    fi.mantissa = (unsigned int)(((unsigned int)num << 9) >> 9);

    return fi;
}

FloatParts Float::get_float_parts(float value)
{
    FloatInfo fi = get_float_info(value);

    FloatParts fp;
    fp.sign = fi.sign;
    fp.displacment = fi.exponent - 127;

    if (fp.displacment < 0)
    {
        fp.whole_part = 0;
    }
    else
    {
        fp.whole_part = (fi.mantissa >> (23 - fp.displacment)) | (1 << fp.displacment);
        if (fp.sign)
            fp.whole_part *= -1;
    }

    fp.fractional_part = value - (float)fp.whole_part;

    return fp;
}
