#pragma once

#include "string/StringBuilder.h"
#include "io/sys_io.h"

template<typename T, typename... Args>
void debugln(string str, T t, Args... args){
    string st = StringBuilder::FormatString(str, t, args...);
    for(char& c: st){
        outb(c, 0xe9);
    }
    outb('\n', 0xe9);
}


template<typename T, typename... Args>
void debug(string str, T t, Args... args){
    string st = StringBuilder::FormatString(str, t, args...);
    for(char& c: st){
        outb(c, 0xe9);
    }
}

void debugln(string st){
    for(char& c: st){
        outb(c, 0xe9);
    }
    outb('\n', 0xe9);
}


void debug(string st){
    for(char& c: st){
        outb(c, 0xe9);
    }
}