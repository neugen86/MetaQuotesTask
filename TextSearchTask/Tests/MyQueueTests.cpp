#include <gtest/gtest.h>

#include "../TextSearchTask/MyQueue.h"

namespace
{
	size_t TEMP_VALUE;
	size_t MAGIC_NUMBER = 13;
	const size_t DEFAULT_AMOUNT = 10;

	MyQueue<size_t> GetQueue(size_t amount, size_t offset = 0)
	{
		MyQueue<size_t> result;

		for (size_t i = 0; i < amount; ++i)
		{
			result.push(i);

			if (offset > i)
				EXPECT_TRUE(result.getNext(&TEMP_VALUE));
		}

		return result;
	}

} // anonymous namespace

TEST(MyQueueTests, Basic)
{
	MyQueue<size_t> queue = GetQueue(0);

	EXPECT_TRUE(queue.empty());
	EXPECT_FALSE(queue.getNext(&TEMP_VALUE));

	queue = GetQueue(DEFAULT_AMOUNT);

	EXPECT_FALSE(queue.empty());
	EXPECT_EQ(queue.size(), DEFAULT_AMOUNT);

	EXPECT_FALSE(queue.getNext(nullptr));
	EXPECT_TRUE(queue.getNext(&TEMP_VALUE));
}

TEST(MyQueueTests, Copy)
{
	MyQueue<size_t> queue = GetQueue(DEFAULT_AMOUNT);

	{
		MyQueue<size_t> copy = GetQueue(MAGIC_NUMBER, MAGIC_NUMBER / 2);
		EXPECT_EQ(copy.size(), MAGIC_NUMBER);

		copy = queue;
		EXPECT_EQ(copy.size(), queue.size());

		for (size_t i = 0; i < DEFAULT_AMOUNT; ++i)
		{
			queue.pop();
		}
		EXPECT_TRUE(queue.empty());

		for (size_t i = 0; i < DEFAULT_AMOUNT; ++i)
		{
			EXPECT_TRUE(copy.getNext(&TEMP_VALUE));
			EXPECT_EQ(TEMP_VALUE, i);
		}
	}
}

TEST(MyQueueTests, Pop)
{
	MyQueue<size_t> queue = GetQueue(DEFAULT_AMOUNT);

	for (size_t i = 0; i < DEFAULT_AMOUNT; ++i)
	{
		EXPECT_EQ(queue.pop(), i);
	}

	EXPECT_TRUE(queue.empty());
}

TEST(MyQueueTests, GetNext)
{
	MyQueue<size_t> queue = GetQueue(DEFAULT_AMOUNT);

	for (size_t i = 0; i < DEFAULT_AMOUNT; ++i)
	{
		EXPECT_TRUE(queue.getNext(&TEMP_VALUE));
		EXPECT_EQ(TEMP_VALUE, i);
	}

	EXPECT_FALSE(queue.getNext(&TEMP_VALUE));

	queue.push(MAGIC_NUMBER);
	EXPECT_TRUE(queue.getNext(&TEMP_VALUE));
	EXPECT_EQ(TEMP_VALUE, MAGIC_NUMBER);

	EXPECT_FALSE(queue.getNext(&TEMP_VALUE));

	queue.reset();
	EXPECT_TRUE(queue.getNext(&TEMP_VALUE));
	EXPECT_EQ(TEMP_VALUE, 0);

	EXPECT_FALSE(queue.empty());
	EXPECT_EQ(queue.size(), DEFAULT_AMOUNT + 1);
}
