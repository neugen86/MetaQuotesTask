#include <gtest/gtest.h>

#include <fstream>

#include "../TextSearchTask/MyQueue.h"
#include "../TextSearchTask/FileReader.h"

namespace
{
	const char* UNKNOWN_FILE_PATH = "unknown";
	const char* DEFAULT_FILE_PATH = "testfile";
	const char* DEFAULT_TEXT = "An algorithm must be seen to be believed";

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
	CreateFile();

	FileReader reader;

	for (size_t i = 0; i < 3; ++i)
	{
		EXPECT_FALSE(reader.open(UNKNOWN_FILE_PATH));
		EXPECT_TRUE(reader.open(DEFAULT_FILE_PATH));
	}
}

TEST(FileReaderTests, Read)
{
	const size_t LINES_COUNT = 1000;

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

TEST(FileReaderTests, InterceptionClose)
{
	const size_t LINES_COUNT = 10;
	const size_t STOP_INDEX = LINES_COUNT / 2;

	EXPECT_TRUE(FillFile(DEFAULT_TEXT, LINES_COUNT));

	MyString line;
	size_t counter = 0;

	FileReader reader;
	EXPECT_TRUE(reader.open(DEFAULT_FILE_PATH));

	while (reader.getNextLine(line))
	{
		if (STOP_INDEX == counter)
			reader.close();

		++counter;
	}

	EXPECT_EQ(counter, STOP_INDEX + 1);
}

TEST(FileReaderTests, InterceptionOpen)
{
	const size_t LINES_COUNT = 10;
	const size_t STOP_INDEX = LINES_COUNT / 2;

	EXPECT_TRUE(FillFile(DEFAULT_TEXT, LINES_COUNT));

	MyString line;
	MyQueue<MyString> lines;

	size_t counter = 0;
	bool stopped = false;

	FileReader reader;
	EXPECT_TRUE(reader.open(DEFAULT_FILE_PATH));

	while (reader.getNextLine(line))
	{
		if (stopped && !lines.empty())
			EXPECT_EQ(line, lines.pop());

		if (counter != STOP_INDEX)
		{
			if (!stopped)
				lines.push(line);
		}
		else
		{
			EXPECT_TRUE(reader.open(DEFAULT_FILE_PATH));
		}

		++counter;
	}

	EXPECT_EQ(counter, LINES_COUNT + STOP_INDEX + 1);
}

TEST(FileReaderTests, InterceptionResetPosition)
{
	const size_t LINES_COUNT = 10;
	const size_t STOP_INDEX = LINES_COUNT / 2;

	EXPECT_TRUE(FillFile(DEFAULT_TEXT, LINES_COUNT));

	MyString line;
	MyQueue<MyString> lines;

	size_t counter = 0;
	bool stopped = false;

	FileReader reader;
	EXPECT_TRUE(reader.open(DEFAULT_FILE_PATH));

	while (reader.getNextLine(line))
	{
		if (stopped && !lines.empty())
			EXPECT_EQ(line, lines.pop());

		if (counter != STOP_INDEX)
		{
			if (!stopped)
				lines.push(line);
		}
		else
		{
			reader.resetPosition();
		}

		++counter;
	}

	EXPECT_EQ(counter, LINES_COUNT + STOP_INDEX + 1);
}
