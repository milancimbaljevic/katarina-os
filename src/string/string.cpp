#include "string.h"

string::string(const char &c)
{
    str[0] = c;
    str[1] = '\0';
    length = 1;
}

string::string(const char *_str)
{
    int len = 0;
    const char *temp = _str;
    while (*temp++)
        len++;
    length = len;

    int i = 0;
    while (i < len && i < MAX_STRING_SIZE)
    {
        str[i] = _str[i];
        i++;
    }
    str[i] = '\0';
}

int string::size() const
{
    return length;
}

const char *string::getCharPointer() const
{
    return str;
}

string string::operator+(const string &other_string)
{
    string new_string(*this);
    int i = length;
    while (i < MAX_STRING_SIZE && i < other_string.length + length)
    {
        new_string.str[i] = other_string.str[i - length];
        i++;
    }
    new_string.length = length + other_string.length;
    new_string.str[i] = '\0';
    return new_string;
}

char string::operator[](int i)
{
    return str[i];
}

string string::substr(int start_index, int number_of_chars)
{
    int i = start_index;
    string res;
    while (i < start_index + number_of_chars && i < length)
    {
        res = res + string(str[i++]);
    }
    return res;
}

string string::reverse()
{
    string res;
    for (int i = length - 1; i >= 0; i--)
    {
        res = res + str[i];
    }
    return res;
}

StringIterator string::begin()
{
    return StringIterator(str);
}

StringIterator string::end()
{
    return StringIterator(&str[MAX_STRING_SIZE]);
}
