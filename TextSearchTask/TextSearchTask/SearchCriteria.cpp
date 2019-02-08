#include "SearchCriteria.h"

#include <cassert>

namespace
{
const char ANY_SYMBOL = '*';
const char SINGLE_SYMBOL = '?';

bool IsMaskSymbol(char value)
{
	switch (value)
    {
    case ANY_SYMBOL:
    case SINGLE_SYMBOL:
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
    MyRange textRange(end);
	
    for (size_t pos = 0; pos < input.length(); ++pos)
    {
		const char symbol = input.at(pos);

        if (IsMaskSymbol(symbol))
        {
			if (hasText)
			{
				end = pos;
				textRange.end = pos;

				break;
			}

			switch (symbol)
			{
			case ANY_SYMBOL:
				result.strictlyCompare = false;
				break;
			
			case SINGLE_SYMBOL:
				++result.minIndent;
				break;

			default:
				assert(false && "Unknown mask symbol");
				break;
			}
        }
        else if (!hasText)
        {
            hasText = true;
            textRange.begin = pos;
        }
    }
	
	result.target = input.substr(textRange);

	return result;
}

} // anonymous namespace

MyQueue<SearchCriteria> SearchCriteria::makeFilter(const MyString& value)
{
    MyQueue<SearchCriteria> result;

	if (!value.contains("\n") && !value.contains("\r"))
	{
		for (MyString tmp = value; tmp.length();)
		{
			size_t end = 0;
			result.push(GetNextCriteria(tmp, end));

			tmp = tmp.substr(MyRange(end, tmp.length()));
		}
	}

    return result;
}

bool SearchCriteria::satisfies(const MyString& input, size_t& endPos) const
{
	if (input.length() < target.length())
		return false;

	MyRange range(input.length());

	if (!target.empty() && !input.contains(target, &range))
		return false;
	
	if (strictlyCompare && (range.begin != minIndent))
		return false;

	if (range.begin < minIndent)
		return false;

	endPos = range.end;

	return true;
}
