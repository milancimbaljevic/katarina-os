#pragma once

#include "string.h"

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
            if(str[i] == '{' && str[i > 0 ? i - 1 : i] != '\\'){
                if(i + 1 >= str.size() || str[i+1] != '}') return "FORMAT '{' ERROR";

                //bool print_in_hex = false;
                
                if(str[i+1] == 'h') {
                  //  print_in_hex = true;
                    i++;
                }

                res = res + to_string(param);
                
                i+=2;

                if constexpr(sizeof...(args) == 0){
                    while(i < str.size()){
                        res = res + str[i++];
                    }
                    return res;
                }else{
                    return res + FormatString(str.substr(i, str.size() - i), args...);
                }

                break;
            }
            res = res + str[i++];
        }

        if(i != str.size()){

        }
    }

    static string to_string(unsigned int num){
        string res;
        while(num != 0){
            unsigned int c = num % 10;
            num /= 10;
            char ch = (char) (c + '0');
            res = res + string(ch);
        }
        return res.reverse();
    }

    static string to_string(int _num){
        int num = _num > 0 ? _num : -_num;
        
        string res;
        while(num != 0){
            unsigned int c = num % 10;
            num /= 10;
            char ch = (char) (c + '0');
            res = res + string(ch);
        }
        if(_num < 0) res = res + string('-');
        return res.reverse();
    }
};