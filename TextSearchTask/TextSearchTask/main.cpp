#include <iostream>

#include "LogReader.h"

int main()
{
    CLogReader reader;
	
	if (!reader.SetFilter("*"))
		return 1;

	const size_t bufsize = 1024;
	char buffer[bufsize];

	while (reader.GetNextLine(buffer, bufsize))
	{
		std::cout << buffer << std::endl;
	}

	std::cout << std::endl << "Press any key..." << std::endl;
	std::getchar();

    return 0;
}
