#include <gtest/gtest.h>

#include <fstream>

#include "../TextSearchTask/FileReader.h"

namespace
{
	const char* UNKNOWN_FILE_PATH = "unknown";
	const char* DEFAULT_FILE_PATH = "testfile";
	const char* DEFAULT_TEXT = "The quick brown fox jumps over the lazy dog";

	bool GetFile(std::fstream& file)
	{
		file.open(
			DEFAULT_FILE_PATH,
			std::ifstream::in | std::ifstream::out | std::ifstream::trunc
		);

		return file.is_open();
	}

	bool CreateFile()
	{
		std::fstream file;
		
		if (!GetFile(file))
			return false;
		
		file.close();
		return true;
	}

	bool FillFile(const char* text, size_t linesCount)
	{
		std::fstream file;

		if (!GetFile(file))
			return false;

		for (size_t i = 0; i < linesCount; ++i)
		{
			file
				<< "#" << (i + 1) << ": "
				<< text << std::endl;
		}

		file.close();
		return true;
	}

} // anonymous namespace

TEST(FileReaderTests, Open)
{
	FileReader reader;
	EXPECT_TRUE(CreateFile());

	for (size_t i = 0; i < 3; ++i)
	{
		EXPECT_FALSE(reader.open(UNKNOWN_FILE_PATH));
		EXPECT_TRUE(reader.open(DEFAULT_FILE_PATH));
	}
}

TEST(FileReaderTests, Intercept)
{
	const size_t LINES_COUNT = 5;
	EXPECT_TRUE(FillFile(DEFAULT_TEXT, LINES_COUNT));

	FileReader reader;
	EXPECT_TRUE(reader.open(DEFAULT_FILE_PATH));

	MyString line;
	size_t counter = 0;
	
	while (reader.getNextLine(line))
	{
		reader.close();
		++counter;
	}
	EXPECT_EQ(counter, 1);

	{
		counter = 0;

		EXPECT_TRUE(reader.open(DEFAULT_FILE_PATH));

		while (reader.getNextLine(line))
		{
			if (counter == LINES_COUNT / 2)
				EXPECT_TRUE(reader.open(DEFAULT_FILE_PATH));

			++counter;
		}
		EXPECT_EQ(counter, LINES_COUNT + LINES_COUNT / 2 + 1);
	}

	{
		counter = 0;

		EXPECT_TRUE(reader.open(DEFAULT_FILE_PATH));

		while (reader.getNextLine(line))
		{
			if (counter == LINES_COUNT / 2)
				reader.resetPosition();

			++counter;
		}
		EXPECT_EQ(counter, LINES_COUNT + LINES_COUNT / 2 + 1);
	}
}

TEST(FileReaderTests, Read)
{
	const size_t LINES_COUNT = 500000;

	EXPECT_TRUE(FillFile(DEFAULT_TEXT, LINES_COUNT));

	FileReader reader;
	EXPECT_TRUE(reader.open(DEFAULT_FILE_PATH));

	MyString line;
	size_t counter = 0;

	while (reader.getNextLine(line))
	{
		++counter;
	}
	EXPECT_EQ(counter, LINES_COUNT);
}
