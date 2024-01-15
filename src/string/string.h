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
    string(const char &c);
    string(const char *_str);
    int size() const;
    const char *getCharPointer() const;
    string operator+(const string &other_string);
    char operator[](int i);
    string substr(int start_index, int number_of_chars);
    string reverse();
    StringIterator begin();
    StringIterator end();
};