#pragma once

#include "FileReader.h"
#include "TextSearch.h"

class CLogReader
{
	FileReader m_reader;
	TextSearch::Filter m_filter;

public:
	~CLogReader();

	bool Open(const char* filePath);
	void Close();

	bool SetFilter(const char* mask);
	bool GetNextLine(char* buf, size_t bufsize);

};