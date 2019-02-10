#include "FileReader.h"

#include <fileapi.h>
#include <memoryapi.h>
#include <handleapi.h>
#include <sysinfoapi.h>

namespace Text
{
	bool isLineBreak(char value)
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

	void skipLineBreaks(FileView& view)
	{
		for (; view.curPos < view.size; ++view.curPos)
		{
			if (!isLineBreak(view.ptr[view.curPos]))
				break;
		}
	}

	void moveToLineBreak(FileView& view)
	{
		for (; view.curPos < view.size; ++view.curPos)
		{
			if (isLineBreak(view.ptr[view.curPos]))
				break;
		}
	}

} // namespace Text

namespace File
{
	DWORD getChunkSize()
	{
		SYSTEM_INFO sysinfo = { 0 };
		::GetSystemInfo(&sysinfo);

		return sysinfo.dwAllocationGranularity;
	}

	FileView getView(HANDLE& handle, unsigned long long fileSize, unsigned long long offset, DWORD size)
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
	: CHUNK_SIZE(File::getChunkSize())
	, m_fileHandle(INVALID_HANDLE_VALUE)
	, m_mappingHandle(INVALID_HANDLE_VALUE)
{
}

FileReader::~FileReader()
{
	close();
}

bool FileReader::open(const char* filePath)
{
	if (m_fileHandle != INVALID_HANDLE_VALUE)
		close();

	m_fileHandle = ::CreateFileA(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if (INVALID_HANDLE_VALUE == m_fileHandle)
	{
		close();
		return false;
	}

	m_mappingHandle = ::CreateFileMapping(m_fileHandle, NULL, PAGE_READONLY, 0, 0, NULL);

	if (INVALID_HANDLE_VALUE == m_mappingHandle)
	{
		close();
		return false;
	}

	LARGE_INTEGER size = { 0 };

	if (::GetFileSizeEx(m_fileHandle, &size) != TRUE)
	{
		close();
		return false;
	}

	m_fileSize = static_cast<unsigned long long>(size.QuadPart);

	return true;
}

void FileReader::close()
{
	resetPosition();

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

	m_fileSize = 0;
}

void FileReader::resetPosition()
{
	if (m_view.ptr)
		::UnmapViewOfFile(m_view.ptr);

	m_view = FileView();
	m_fileOffset = 0;
}

bool FileReader::getNextLine(MyString& value)
{
	if (m_fileOffset >= m_fileSize)
		return false;

	if (!m_view.size && !loadFileView())
		return false;

	Text::skipLineBreaks(m_view);

	size_t fromPos = m_view.curPos;
	Text::moveToLineBreak(m_view);

	value = MyString(m_view.ptr + fromPos, (m_view.curPos - fromPos));

	if (size_t(m_view.curPos) == m_view.size)
	{
		m_fileOffset += m_view.size;

		const char lastSymbol = m_view.ptr[m_view.curPos - 1];

		if (loadFileView() && !Text::isLineBreak(lastSymbol))
		{
			Text::moveToLineBreak(m_view);

			MyString tail(m_view.ptr, m_view.curPos);
			value.append(tail);
		}
	}

	if (value.empty())
		return getNextLine(value);

	return true;
}

bool FileReader::loadFileView()
{
	if (m_view.ptr)
		::UnmapViewOfFile(m_view.ptr);

	m_view = File::getView(m_mappingHandle, m_fileSize, m_fileOffset, CHUNK_SIZE);

	return (m_view.size > 0);
}
