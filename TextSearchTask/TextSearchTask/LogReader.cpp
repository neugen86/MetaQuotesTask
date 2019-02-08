#include "LogReader.h"

CLogReader::~CLogReader()
{
	Close();
}

bool CLogReader::Open(const char* filePath)
{
	return m_reader.Open(filePath);
}

void CLogReader::Close()
{
	m_reader.Close();
}

bool CLogReader::SetFilter(const char* mask)
{
	m_filter = SearchCriteria::makeFilter(mask);

	return !m_filter.empty();
}

bool CLogReader::GetNextLine(char* buf, const int length)
{
	if (m_filter.empty())
		return false;

	MyString line;

	while (m_reader.GetNextLine(line))
	{
		m_filter.reset();

		bool match = false;
		size_t lastTextPos = 0;

		SearchCriteria criteria;

		while (m_filter.getNext(&criteria))
		{
			MyRange substrRange(lastTextPos, line.length());

			if (!substrRange.length())
				break;

			size_t lastSubstrPos = 0;
			MyString substr = line.substr(substrRange);

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
			size_t amount = (int(line.length()) - length) < 0 ? line.length() : (length - 1);
			strncpy_s(buf, length, line.data(), amount);

			return true;
		}
	}

	return false;
}
