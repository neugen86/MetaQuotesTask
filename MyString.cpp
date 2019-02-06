#include "MyString.h"

#include <cstring>
#include <utility>

MyString::MyString(const char* source)
    : MyString(source, source ? strlen(source) : 0)
{
}

MyString::MyString(const char* source, size_t length)
    : m_value(source)
    , m_length(length)
{
}

MyString::MyString(const MyString& other)
    : m_isCopy(true)
    , m_value(new char[other.length() + 1])
    , m_length(other.length())
{
    strncpy(const_cast<char*>(m_value), other.m_value, other.length());
    const_cast<char&>(m_value[m_length]) = '\0';
}

MyString::~MyString()
{
    if (m_isCopy)
        delete[] m_value;
}

MyString& MyString::operator =(const MyString& other)
{
    if (this != &other)
    {
        MyString tmp(other);
        swap(*this, tmp);

        m_isCopy = true;
    }

    return *this;
}

bool MyString::contains(const MyString& substr, Range* range) const
{
    bool found = false;
    size_t substrPos = 0;

    if (range)
        range->begin = range->end = m_length;

    for (size_t pos = 0; pos < m_length; ++pos)
    {
        if (at(pos) == substr.at(substrPos))
        {
            if (!found)
            {
                found = true;

                if (range)
                    range->begin = pos;
            }
        }
        else if (found)
        {
            if (range)
                range->end = pos;

            return false;
        }

        if (found)
        {
            if (++substrPos == substr.length())
            {
                if (range)
                    range->end = (pos + 1);

                return true;
            }
        }
    }

    return false;
}

MyString MyString::substr(const MyString::Range& range) const
{
    if ((range.begin < range.end)
        && (range.end <= length()))
    {
        return MyString(m_value + range.begin, range.end - range.begin);
    }

    return MyString();
}

void MyString::swap(MyString& lhs, MyString& rhs) const
{
    using std::swap;

    swap(lhs.m_value, rhs.m_value);
    swap(lhs.m_length, rhs.m_length);
}
