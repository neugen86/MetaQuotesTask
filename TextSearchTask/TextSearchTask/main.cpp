#include <iostream>

#include "LogReader.h"

void SearchAll(CLogReader& reader)
{
	const size_t bufsize = 1024;
	char buffer[bufsize];

	std::cout << "Search results:" << std::endl << std::endl;

	while (reader.GetNextLine(buffer, bufsize))
	{
		std::cout << buffer << std::endl;
	}
}

int main(/*int argc, char* argv[]*/)
{
	const int argc = 3;
	const char* argv[] = { "main", "test.txt", "* ????? *" };

	if (argc < 3)
	{
		std::cout
			<< "Wrong arguments" << std::endl
			<< "Usage: " << argv[0] << " file_path search_mask" << std::endl;
		
		return 1;
	}
	
	CLogReader reader;

	if (!reader.Open(argv[1]))
	{
		std::cout << "File open error" << std::endl;
		return 2;
	}
	
	if (!reader.SetFilter(argv[2]))
	{
		std::cout << "Invalid search mask" << std::endl;
		return 3;
	}

	SearchAll(reader);

	std::cout << std::endl << "Press <ENTER> for exit...";
	std::getchar();

    return 0;
}
