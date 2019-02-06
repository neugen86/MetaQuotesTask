#pragma once

#include <MyQueue.h>
#include <MyString.h>

struct SearchCriteria
{
    MyString target;
    int minIndent = -1;

    static MyQueue<SearchCriteria> parseMask(const MyString& value);

};
