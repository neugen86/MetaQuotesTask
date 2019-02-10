#pragma once

#include "MyQueue.h"
#include "MyString.h"

namespace TextSearch
{
	struct Criteria
	{
		MyString target;
		size_t minIndent = 0;
		bool strictlyCompare = true;

		bool satisfies(const MyString& input, MyRange& range) const;

	};

	using Filter = MyQueue<Criteria>;

	Filter makeFilter(const MyString& value);
	bool isMatches(const MyString& line, const Filter& filter);

	bool operator !=(const Criteria& lhs, const Criteria& rhs);
	bool operator ==(const Criteria& lhs, const Criteria& rhs);

} // namespace TextSearch
