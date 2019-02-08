#pragma once

#include "SearchCriteria.h"

class CLogReader
{
	mutable size_t m_lastPos = 0;

	MyQueue<SearchCriteria> m_filter;

public:
	explicit CLogReader();
	~CLogReader();

	bool Open(const char* filePath);
	void Close();

	bool SetFilter(const char* mask);
	bool GetNextLine(char* buf, const int length);

private:
	bool GetNextString(MyString& result) const;

};