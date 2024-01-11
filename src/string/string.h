#pragma once

#define MAX_STRING_SIZE 200

struct StringIterator
{
    StringIterator(char *ptr) : char_ptr(ptr) {}
    char &operator*() { return *char_ptr; }
    // char* operator->() { return char_ptr; }
    StringIterator &operator++()
    {
        char_ptr++;
        return *this;
    }
    StringIterator operator++(int)
    {
        StringIterator temp = *this;
        ++char_ptr; // (*this)++
        return temp;
    }
    friend bool operator==(const StringIterator &it1, const StringIterator &it2)
    {
        return it1.char_ptr == it2.char_ptr;
    }
    friend bool operator!=(const StringIterator &it1, const StringIterator &it2)
    {
        return it1.char_ptr != it2.char_ptr;
    }

private:
    char *char_ptr;
};

class string
{
private:
    char str[MAX_STRING_SIZE + 1];
    int length = 0;

public:
    string() = default;

    string(const char &c)
    {
        str[0] = c;
        str[1] = '\0';
        length = 1;
    }

    string(const char *_str)
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

    int size() const
    {
        return length;
    }

    const char *getCharPointer() const
    {
        return str;
    }

    string operator+(const string &other_string)
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

    char operator[](int i)
    {
        return str[i];
    }

    string substr(int start_index, int number_of_chars)
    {
        int i = start_index;
        string res;
        while (i < start_index + number_of_chars && i < length)
        {
            res = res + string(str[i++]);
        }
        return res;
    }

    string reverse()
    {
        string res;
        for (int i = length - 1; i >= 0; i--)
        {
            res = res + str[i];
        }
        return res;
    }

    StringIterator begin()
    {
        return StringIterator(str);
    }

    StringIterator end()
    {
        return StringIterator(&str[MAX_STRING_SIZE]);
    }
};