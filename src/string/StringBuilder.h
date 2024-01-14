#pragma once

#include "string.h"
//#include "utils/float.h"

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

    static string to_string(float num){
        return "";
    }

    static string to_string(unsigned int num)
    {
        if(num == 0) return '0';

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
        if(_num == 0) return '0';

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

    // static string to_string_binary(float num)
    // {
    //     ufloat u1;
    //     u1.f = num;

    //     return to_string_binary(u1.u);
    // }
};