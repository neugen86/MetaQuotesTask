#include "SearchCriteria.h"

namespace
{
bool IsMaskSymbol(char value)
{
    switch (value)
    {
    case '*':
    case '?':
        return true;

    default:
        return false;
    }
}

SearchCriteria GetNextCriteria(const MyString& input, size_t& end)
{
    SearchCriteria result;

    end = input.length();

    bool hasText = false;
    MyString::Range textRange;

    for (size_t pos = 0; pos < input.length(); ++pos)
    {
        if (IsMaskSymbol(input.at(pos)))
        {
            if (hasText)
            {
                end = pos;

                textRange.end = pos;
                result.target = input.substr(textRange);

                return result;
            }

            if (input.at(pos) == '?')
            {
                if (result.minIndent < 0)
                    result.minIndent = 0;

                ++result.minIndent;
            }
        }
        else if (!hasText)
        {
            hasText = true;
            textRange.begin = pos;
        }
    }

    return result;
}

} // anonymous namespace

MyQueue<SearchCriteria> SearchCriteria::parseMask(const MyString& value)
{
    MyQueue<SearchCriteria> result;

    for (MyString tmp = value; tmp.length();)
    {
        size_t end = 0;
        result.push(GetNextCriteria(tmp, end));

        tmp = tmp.substr(MyString::Range(end, tmp.length()));
    }

    return result;
}
