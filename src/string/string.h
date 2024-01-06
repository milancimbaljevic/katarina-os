#pragma once

#define MAX_STRING_SIZE 20

class string{
private:
    char str[MAX_STRING_SIZE+1];
    int length = 0;
public:
    string(const char* _str){
        int len = 0;
        const char* temp = _str;
        while(*temp++) len++;
        length = len;

        int i = 0;
        while(i < len && i < MAX_STRING_SIZE){
            str[i] = _str[i];
            i++;
        }
        str[i] = '\0';
    }

    int size(){
        return length;
    }

    char* getCharPointer(){
        return str;
    }
};