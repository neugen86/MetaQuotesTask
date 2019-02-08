#include "LogReader.h"

#include <cassert>

namespace
{
//const char* const TEST_STRING = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

	const char* const TEST_STRING =
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit,\r\n"
		"sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n"
		"Ut enim ad minim veniam, quis nostrud exercitation ullamco\r\n"
		"laboris nisi ut aliquip ex ea commodo consequat.\n"
		"Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore\r\n"
		"eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident,\n"
		"sunt in culpa qui officia deserunt mollit anim id est laborum.\r\n";

} // anonymous namespace

CLogReader::CLogReader()
{
}

CLogReader::~CLogReader()
{
}

bool CLogReader::Open(const char* filePath)
{
	return true;
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

	MyString string;

	while (GetNextString(string))
	{
		m_filter.reset();

		bool match = false;
		size_t lastTextPos = 0;
		
		SearchCriteria criteria;

		while (m_filter.getNext(&criteria))
		{
			MyRange substrRange(lastTextPos, string.length());

			if (!substrRange.length())
				break;
		
			size_t lastSubstrPos = 0;
			MyString substr = string.substr(substrRange);

			if (criteria.satisfies(substr, lastSubstrPos))
			{
				lastTextPos += lastSubstrPos;
				match = true;
			}
			else if (match)
			{
				m_filter.reset();
				match = false;
			}
			else
			{
				break;
			}
		}

		if (match)
		{
			size_t amount = (int(string.length()) - length) < 0 ? string.length() : length;
			strncpy_s(buf, length, string.data(), amount);

			return true;
		}
	}
		
	return false;
}

bool CLogReader::GetNextString(MyString& result) const
{
	const size_t maxLength = strlen(TEST_STRING);

	if (m_lastPos >= maxLength)
		return false;

	for (; m_lastPos < maxLength; ++m_lastPos)
	{
		const char symbol = TEST_STRING[m_lastPos];

		if ((symbol != '\r') && (symbol != '\n'))
			break;
	}

	size_t pos = m_lastPos;

	for (; m_lastPos < maxLength; ++m_lastPos)
	{
		const char symbol = TEST_STRING[m_lastPos];

		if ((symbol == '\r') || (symbol == '\n'))
		{
			break;
		}
	}

	result = MyString(TEST_STRING + pos, (m_lastPos - pos));

	return !result.empty();
}