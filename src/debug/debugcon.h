#pragma once

#include "string/StringBuilder.h"
#include "io/sys_io.h"

template<typename T, typename... Args>
void debugln(string str, T t, Args... args);
template<typename T, typename... Args>
void debug(string str, T t, Args... args);
void debugln(string st);
void debug(string st);