#include "TextSearch.h"

namespace
{
	const char ANY_SYMBOL = '*';
	const char SINGLE_SYMBOL = '?';

	bool isMaskSymbol(char value)
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

	TextSearch::Criteria getNextCriteria(const MyString& input, size_t& end)
	{
		TextSearch::Criteria result;
		end = input.length();

		bool hasText = false;
		MyRange textRange(end);

		for (size_t pos = 0; pos < input.length(); ++pos)
		{
			const char symbol = input.at(pos);

			if (isMaskSymbol(symbol))
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

namespace TextSearch
{
	bool Criteria::satisfies(const MyString& input, MyRange& range) const
	{
		range = MyRange(input.length());

		if (input.length() < target.length())
			return false;

		if (!target.empty() && !input.contains(target, &range))
			return false;

		if (strictlyCompare && (range.begin != minIndent))
			return false;

		if (range.begin < minIndent)
			return false;

		return true;
	}

	Filter makeFilter(const MyString& value)
	{
		Filter result;

		if (!value.contains("\n") && !value.contains("\r"))
		{
			for (MyString tmp = value; tmp.length();)
			{
				size_t end = 0;
				result.push(getNextCriteria(tmp, end));

				tmp = tmp.substr(MyRange(end, tmp.length()));
			}
		}

		return result;
	}

	bool isMatches(const MyString& line, const Filter& filter)
	{
		filter.reset();

		bool result = false;
		size_t lastTextPos = 0;

		Criteria criteria;

		while (filter.getNext(&criteria))
		{
			MyRange substrRange(lastTextPos, line.length());
			MyString substr = line.substr(substrRange);

			MyRange criteriaRange;

			if (criteria.satisfies(substr, criteriaRange))
			{
				lastTextPos += criteriaRange.end;
				result = true;
			}
			else if (result)
			{
				filter.reset();
				result = false;
			}
			else
			{
				break;
			}
		}

		return result;
	}

	bool operator !=(const Criteria& lhs, const Criteria& rhs)
	{
		return
			(lhs.strictlyCompare != rhs.strictlyCompare)
			|| (lhs.minIndent != rhs.minIndent)
			|| (lhs.target != rhs.target);
	}

	bool operator ==(const Criteria& lhs, const Criteria& rhs)
	{
		return !(lhs != rhs);
	}

} // namespace TextSearch
