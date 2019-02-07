#pragma once

#include "MyQueue.h"
#include "MyString.h"

struct SearchCriteria
{
    MyString target;
    size_t minIndent = 0;
	bool strictlyCompare = true;

	using Filter = MyQueue<SearchCriteria>;

    static Filter makeFilter(const MyString& value);

	bool satisfies(const MyString& input, size_t& endPos) const;

};
