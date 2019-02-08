#pragma once

#include "FileReader.h"
#include "SearchCriteria.h"

class CLogReader
{
	FileReader m_reader;
	MyQueue<SearchCriteria> m_filter;

public:
	~CLogReader();

	bool Open(const char* filePath);
	void Close();

	bool SetFilter(const char* mask);
	bool GetNextLine(char* buf, const int length);

};