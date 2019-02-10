#include <iostream>

#include "consoleapi.h"

#include "LogReader.h"

namespace
{
	bool _stop = false;

	BOOL WINAPI StopSearch(DWORD)
	{
		_stop = true;
		return true;
	}

	bool Init(CLogReader& reader, const char* filePath, const char* searchMask)
	{
		std::cout << "File Path: " << filePath << std::endl;
		std::cout << "Search Mask: " << searchMask << std::endl;

		if (!reader.Open(filePath))
		{
			std::cout << "File open error" << std::endl;
			return false;
		}

		if (!reader.SetFilter(searchMask))
		{
			std::cout << "Invalid search mask" << std::endl;
			return false;
		}

		::SetConsoleCtrlHandler(&StopSearch, TRUE);

		return true;
	}

	void SearchAll(CLogReader& reader)
	{
		const size_t bufsize = 1024;
		char buffer[bufsize];

		std::cout << "Search results:" << std::endl;

		while (!_stop && reader.GetNextLine(buffer, bufsize))
		{
			std::cout << std::endl << buffer << std::endl;
		}
	}

} // anonymous namespace

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "Wrong arguments!" << std::endl
			<< "Usage: \"path_to_file\" \"search_mask\"" << std::endl;

		return 1;
	}

	CLogReader reader;

	if (!Init(reader, argv[1], argv[2]))
		return 2;

	SearchAll(reader);

	std::cout << std::endl << "Press <ENTER> for exit...";
	std::getchar();

    return 0;
}
