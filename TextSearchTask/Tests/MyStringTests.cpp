#include <gtest/gtest.h>

#include "../TextSearchTask/MyString.h"

namespace
{
	const size_t DEFAULT_SIZE = 10;
	const char* TEST_CHARS = "Premature optimization is the root of all evil";

	const char* GetChars(size_t size)
	{
		char* buf = new char[size + 1];

		for (size_t i = 0; i < size; ++i)
		{
			buf[i] = char((i + 97) % 122);
		}

		buf[size] = '\0';

		return buf;
	}

} // anonymous namespace


TEST(MyStringTests, Empty)
{
	MyString string;

	EXPECT_TRUE(string.empty());
	EXPECT_EQ(string.length(), 0);
	EXPECT_EQ(string.data(), nullptr);
}

TEST(MyStringTests, CtorPtr)
{
	MyString string(TEST_CHARS);

	EXPECT_FALSE(string.empty());
	EXPECT_EQ(string.data(), TEST_CHARS);
	EXPECT_EQ(string.length(), strlen(TEST_CHARS));
}

TEST(MyStringTests, CtorPtrLength)
{
	const size_t pos = 5;
	const size_t length = 6;

	MyString string(TEST_CHARS + pos, length);

	EXPECT_FALSE(string.empty());
	EXPECT_EQ(string.data(), TEST_CHARS + pos);
	EXPECT_EQ(string.length(), length);
}

TEST(MyStringTests, Equals)
{
	MyString s1(TEST_CHARS);
	MyString s2(TEST_CHARS);
	MyString s3(TEST_CHARS, s2.length() - 1);
	MyString s4(TEST_CHARS + 1, s3.length());

	EXPECT_EQ(s1, s2);
	EXPECT_NE(s2, s3);
	EXPECT_NE(s3, s4);
}

TEST(MyStringTests, Iterating)
{
	MyString string(TEST_CHARS);

	for (size_t i = 0; i < string.length(); ++i)
	{
		EXPECT_EQ(string.at(i), *(TEST_CHARS + i));
	}
}

TEST(MyStringTests, Copy)
{
	const char* buf = GetChars(DEFAULT_SIZE);

	MyString view(buf);
	MyString anotherView(buf);

	EXPECT_EQ(view.length(), DEFAULT_SIZE);

	EXPECT_EQ(view.data(), anotherView.data());
	EXPECT_EQ(view.length(), anotherView.length());

	{
		MyString* copy = new MyString();
		*copy = view;

		MyString* anotherCopy = new MyString(*copy);

		EXPECT_EQ(view.length(), copy->length());
		EXPECT_EQ(view.length(), anotherCopy->length());

		EXPECT_NE(view.data(), copy->data());
		EXPECT_NE(view.data(), anotherCopy->data());
		EXPECT_NE(copy->data(), anotherCopy->data());

		EXPECT_NO_FATAL_FAILURE(delete copy);
		EXPECT_NO_FATAL_FAILURE(delete anotherCopy);
	}

	EXPECT_NO_FATAL_FAILURE(delete[] buf);
}

TEST(MyStringTests, AppendContains)
{
	const char* const SPACE = " ";
	const size_t WORDS_AMOUNT = 3;
	const char* const UNKNOWN_WORD = "unknown";
	const char* const WORDS[WORDS_AMOUNT] = { "foo", "foobar", "foobarbaz" };

	size_t length = 0;
	MyRange ranges[WORDS_AMOUNT];

	MyString string;

	for (size_t i = 0; i < WORDS_AMOUNT; ++i)
	{
		const char* word = WORDS[i];

		string.append(word);
		string.append(SPACE);

		ranges[i].begin = length;
		ranges[i].end = length + strlen(word);

		length += strlen(word) + strlen(SPACE);
	}
	EXPECT_EQ(string.length(), length);

	EXPECT_FALSE(string.contains(UNKNOWN_WORD));

	for (size_t i = 0; i < WORDS_AMOUNT; ++i)
	{
		MyRange range;
		const MyRange& knownRange = ranges[i];

		EXPECT_TRUE(string.contains(WORDS[i], &range));		
		EXPECT_EQ(range, knownRange);
	}
}

TEST(MyStringTests, SubstrCopy)
{
	const MyString SPACE(" ");

	const size_t BUF_SIZE = 4;
	const MyRange BUF_RANGE(0, BUF_SIZE - 1);

	const char* DATA = "Theese are not the droids you looking for";
	const size_t DATA_LENGTH = strlen(DATA);

	size_t pos = 0;
	char buf[BUF_SIZE] = {};

	while (pos < DATA_LENGTH)
	{
		MyRange range;
		MyString string = MyString(DATA + pos);

		if (!string.contains(SPACE, &range))
			EXPECT_EQ(range.begin, string.length());

		MyRange substrRange(0, range.begin);
		MyString substr = string.substr(substrRange);

		EXPECT_FALSE(substr.empty());
		EXPECT_EQ(substr.length(), substrRange.length());

		EXPECT_NO_FATAL_FAILURE(substr.copy(buf, BUF_SIZE));

		MyString substrCopy(buf);
		EXPECT_EQ(substrCopy.length(), (BUF_SIZE - 1));

		MyRange copyRange;
		EXPECT_TRUE(substr.contains(substrCopy, &copyRange));
		EXPECT_EQ(copyRange, BUF_RANGE);

		pos += range.end;
	}
}