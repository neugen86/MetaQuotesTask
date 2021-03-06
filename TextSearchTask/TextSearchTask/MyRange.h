#pragma once

struct MyRange
{
	size_t begin = 0;
	size_t end = 0;

	MyRange() = default;

	MyRange(size_t initial)
		: begin(initial)
		, end(initial)
	{
	}

	MyRange(size_t begin, size_t end)
		: begin(begin)
		, end(end)
	{
	}

	size_t length() const
	{
		return ((long long(end) - begin) > 0) ? (end - begin) : 0;
	}

};

bool operator !=(const MyRange& lhs, const MyRange& rhs);
bool operator ==(const MyRange& lhs, const MyRange& rhs);
