#include "LogReader.h"

#include <cassert>

namespace
{
const char* const TEST_STRING = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

} // anonymous namespace

CLogReader::CLogReader()
{
}

CLogReader::~CLogReader()
{
}

bool CLogReader::Open()
{
	return false;
}

void CLogReader::Close()
{
}

bool CLogReader::SetFilter(const char* mask)
{
	m_lastPos = 0;
	m_filter = SearchCriteria::makeFilter(mask);
	
	return !m_filter.empty();
}

bool CLogReader::GetNextLine(char* buf, const int length)
{
	if (m_filter.empty())
		return false;

	MyString word;
	
	while (GetNextWord(word))
	{
		bool satisfies = false;
		size_t lastTextPos = 0;

		SearchCriteria::Filter filter = m_filter;

		while (!filter.empty())
		{
			SearchCriteria criteria = filter.pop();

			size_t lastSubstrPos = 0;
			MyString substr = word.substr(MyRange(lastTextPos, word.length()));

			satisfies = criteria.satisfies(substr, lastSubstrPos);
			lastTextPos += lastSubstrPos;
			
			if (!satisfies)
				break;
		}

		if (satisfies)
		{
			size_t amount = (int(word.length()) - length) < 0 ? word.length() : length;
			strncpy_s(buf, length, word.data(), amount);
			
			return true;
		}
	}
		
	return false;
}

bool CLogReader::GetNextWord(MyString& result) const
{
	const size_t maxLength = strlen(TEST_STRING);

	if (m_lastPos >= maxLength)
		return false;

	for (; m_lastPos < maxLength; ++m_lastPos)
	{
		if (TEST_STRING[m_lastPos] != ' ')
			break;
	}

	size_t pos = m_lastPos;

	for (; m_lastPos < maxLength; ++m_lastPos)
	{
		if (TEST_STRING[m_lastPos] == ' ')
			break;
	}

	result = MyString(TEST_STRING + pos, m_lastPos - pos);

	++m_lastPos;

	return !result.empty();
}