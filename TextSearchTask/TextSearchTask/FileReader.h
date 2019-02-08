#pragma once

#define _X86_

#include <minwindef.h>

#include "MyString.h"

struct FileView
{
	size_t size = 0;
	size_t curPos = 0;
	const char* ptr = nullptr;

	FileView() = default;

	FileView(const char* ptr, DWORD size)
		: ptr(ptr), size(size) {}
};

class FileReader
{
	const DWORD CHUNK_SIZE;

	FileView m_view;

	HANDLE m_fileHandle;
	HANDLE m_mappingHandle;

	unsigned long long m_fileSize = 0;
	unsigned long long m_fileOffset = 0;

public:
	FileReader();
	~FileReader();

	bool Open(const char* filePath);
	void Close();

	void ResetPosition();
	bool GetNextLine(MyString& value);

private:
	bool GetCurrentView();

};
