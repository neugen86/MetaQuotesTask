#include "FileReader.h"

#include <fileapi.h>
#include <memoryapi.h>
#include <handleapi.h>
#include <sysinfoapi.h>

namespace Text
{
	bool IsLineBreak(char value)
	{
		switch (value)
		{
		case '\r':
		case '\n':
			return true;

		default:
			return false;
		}
	}

	void SkipLineBreaks(FileView& view)
	{
		for (; view.curPos < view.size; ++view.curPos)
		{
			if (!IsLineBreak(view.ptr[view.curPos]))
				break;
		}
	}

	void MoveToLineBreak(FileView& view)
	{
		for (; view.curPos < view.size; ++view.curPos)
		{
			if (IsLineBreak(view.ptr[view.curPos]))
				break;
		}
	}

} // namespace Text

namespace File
{
	DWORD GetChunkSize()
	{
		SYSTEM_INFO sysinfo = { 0 };
		::GetSystemInfo(&sysinfo);

		return sysinfo.dwAllocationGranularity;
	}

	FileView GetView(HANDLE& handle, unsigned long long fileSize, unsigned long long offset, DWORD size)
	{
		unsigned long long maxSize = ((offset + size) > fileSize) ? (fileSize - offset) : size;

		const DWORD high = static_cast<DWORD>((offset >> 32) & 0xFFFFFFFFul);
		const DWORD low = static_cast<DWORD>(offset & 0xFFFFFFFFul);
		const DWORD num = static_cast<DWORD>(maxSize);

		const LPVOID view = ::MapViewOfFile(handle, FILE_MAP_READ, high, low, num);
		
		return FileView(static_cast<const char*>(view), static_cast<size_t>(num));
	}

} // namespace File

FileReader::FileReader()
	: CHUNK_SIZE(File::GetChunkSize())
	, m_fileHandle(INVALID_HANDLE_VALUE)
{
}

FileReader::~FileReader()
{
	Close();
}

bool FileReader::Open(const char* filePath)
{
	m_fileHandle = ::CreateFileA(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if (INVALID_HANDLE_VALUE == m_fileHandle)
	{
		Close();
		return false;
	}
		
	m_mappingHandle = ::CreateFileMappingW(m_fileHandle, NULL, PAGE_READONLY, 0, 0, NULL);

	if (INVALID_HANDLE_VALUE == m_mappingHandle)
	{
		Close();
		return false;
	}

	LARGE_INTEGER size = { 0 };

	if (::GetFileSizeEx(m_fileHandle, &size) != TRUE)
	{
		Close();
		return false;
	}

	m_fileSize = static_cast<unsigned long long>(size.QuadPart);

	return true;
}

void FileReader::Close()
{
	if (m_mappingHandle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_mappingHandle);
		m_mappingHandle = INVALID_HANDLE_VALUE;
	}

	if (m_fileHandle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_fileHandle);
		m_fileHandle = INVALID_HANDLE_VALUE;
	}

	ResetPosition();
	m_fileSize = 0;
}

void FileReader::ResetPosition()
{
	m_fileOffset = 0;
	m_view = FileView();
}

bool FileReader::GetNextLine(MyString& value)
{
	if (m_fileOffset == m_fileSize)
		return false;

	if (!m_view.size && !GetCurrentView())
		return false;

	Text::SkipLineBreaks(m_view);

	size_t fromPos = m_view.curPos;
	Text::MoveToLineBreak(m_view);

	value = MyString(m_view.ptr + fromPos, (m_view.curPos - fromPos));

	if (size_t(m_view.curPos) == m_view.size)
	{
		m_fileOffset += m_view.size;

		const char symbol = m_view.ptr[m_view.curPos - 1];

		if (!Text::IsLineBreak(symbol) && GetCurrentView())
		{
			Text::MoveToLineBreak(m_view);

			MyString tail(m_view.ptr, m_view.curPos);
			value.append(tail);
		}
	}

	return !value.empty();
}

bool FileReader::GetCurrentView()
{
	m_view = File::GetView(m_mappingHandle, m_fileSize, m_fileOffset, CHUNK_SIZE);
	
	return (m_view.size > 0);
}
