#include <iostream>

#include <MyString.h>
#include <SearchCriteria.h>

int main()
{
    MyString source("qwertyasdfghzxcvb");

    MyString::Range range;
    const bool result = source.contains("asd", &range);
    (void)result;

    MyString mask("*?*?*?*?*QwE*?*?*?*?*");
    MyQueue<SearchCriteria> items = SearchCriteria::parseMask(mask);

    return 0;
}
