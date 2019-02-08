#pragma once

#include "MyRange.h"

class MyString
{
    bool m_isCopy = false;
    const char* m_data = nullptr;
    size_t m_length = 0;

public:
    MyString(const char* source = nullptr);
    MyString(const char* source, size_t length);

    MyString(const MyString& other);

    ~MyString();

    MyString& operator =(const MyString& other);

	const char* data() const { return m_data; }

    size_t length() const { return m_length; }
    char at(size_t index) const { return *(m_data + index); }

	bool empty() const { return length() == 0; }
    bool contains(const MyString& substr, MyRange* range = nullptr) const;

    MyString substr(const MyRange& range) const;
	void append(const MyString& tail);

private:
    void swap(MyString& lhs, MyString& rhs) const;

};
