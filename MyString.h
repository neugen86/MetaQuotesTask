#pragma once

class MyString
{
    bool m_isCopy = false;
    const char* m_value = nullptr;
    size_t m_length = 0;

public:
    struct Range
    {
        size_t begin = 0;
        size_t end = 0;

        Range() = default;

        Range(size_t initial)
            : begin(initial)
            , end(initial)
        {
        }

        Range(size_t begin, size_t end)
            : begin(begin)
            , end(end)
        {
        }
    };

    MyString(const char* source = nullptr);
    MyString(const char* source, size_t length);

    MyString(const MyString& other);

    ~MyString();

    MyString& operator =(const MyString& other);

    size_t length() const { return m_length; }
    char at(size_t index) const { return *(m_value + index); }

    bool contains(const MyString& substr, Range* range = nullptr) const;

    MyString substr(const MyString::Range& range) const;

private:
    void swap(MyString& lhs, MyString& rhs) const;

};
