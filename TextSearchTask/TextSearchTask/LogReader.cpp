#include "LogReader.h"

CLogReader::~CLogReader()
{
	Close();
}

bool CLogReader::Open(const char* filePath)
{
	return m_reader.open(filePath);
}

void CLogReader::Close()
{
	m_reader.close();
}

bool CLogReader::SetFilter(const char* mask)
{
	m_filter = TextSearch::makeFilter(mask);
	return !m_filter.empty();
}

bool CLogReader::GetNextLine(char* buf, size_t bufsize)
{
	if (m_filter.empty())
		return false;

	MyString line;

	while (m_reader.getNextLine(line))
	{
		if (TextSearch::isMatches(line, m_filter))
		{
			line.copy(buf, bufsize);
			return true;
		}
	}

	return false;
}
