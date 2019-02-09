#include "MyString.h"

#include <cstring>
#include <utility>

bool operator !=(const MyString& lhs, const MyString& rhs)
{
	if (lhs.length() != rhs.length())
		return true;

	for (size_t i = 0; i < lhs.length(); ++i)
	{
		if (lhs.at(i) != rhs.at(i))
			return true;
	}

	return false;
}

bool operator ==(const MyString& lhs, const MyString& rhs)
{
	return !(lhs != rhs);
}

MyString::MyString(const char* source)
    : MyString(source, source ? strlen(source) : 0)
{
}

MyString::MyString(const char* source, size_t length)
    : m_data(source)
    , m_length(length)
{
}

MyString::MyString(const MyString& other)
    : m_isCopy(true)
    , m_data(new char[other.m_length + 1])
    , m_length(other.m_length)
{
    strncpy_s(
		const_cast<char*>(m_data), m_length + 1,
		other.m_data, other.m_length
	);
}

MyString::~MyString()
{
    if (m_isCopy)
        delete[] m_data;
}

MyString& MyString::operator =(const MyString& other)
{
    if (this != &other)
    {
        MyString tmp(other);
        swap(*this, tmp);
    }

    return *this;
}

bool MyString::contains(const MyString& substr, MyRange* range) const
{
    bool found = false;
    size_t substrPos = 0;

	if (range)
		*range = MyRange(m_length);

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
			found = false;
			substrPos = 0;

            if (range)
                range->end = pos;

			--pos;
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

void MyString::append(const MyString& tail)
{
	const size_t total = (m_length + tail.m_length + 1);

	char* data = new char[total];
	strncpy_s(data, total, m_data, m_length);

	strncpy_s(
		data + m_length, total - m_length,
		tail.m_data, tail.m_length
	);

	MyString tmp(data);
	swap(*this, tmp);

	m_isCopy = true;
}

MyString MyString::substr(const MyRange& range) const
{
	if ((range.begin < range.end) && (range.end <= m_length))
		return MyString(m_data + range.begin, range.length());

	return MyString();
}

void MyString::copy(char* buf, size_t bufsize) const
{
	size_t amount = (long long(m_length) - bufsize) < 0 ? m_length : (bufsize - 1);
	strncpy_s(buf, bufsize, m_data, amount);
}

void MyString::swap(MyString& lhs, MyString& rhs) const
{
    using std::swap;

    swap(lhs.m_data, rhs.m_data);
    swap(lhs.m_length, rhs.m_length);
	swap(lhs.m_isCopy, rhs.m_isCopy);
}
