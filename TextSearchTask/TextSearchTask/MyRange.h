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

	size_t length() const { return (end > begin) ? (end - begin) : 0; }

};