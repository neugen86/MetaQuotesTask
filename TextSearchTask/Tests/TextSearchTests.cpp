#include <gtest/gtest.h>

#include "../TextSearchTask/TextSearch.h"

namespace
{
	const char* DEFAULT_TARGET = "target";

	TextSearch::Criteria GetCriteria(const MyString& target, size_t minIndent, bool strictlyCompare)
	{
		TextSearch::Criteria criteria;
		{
			criteria.target = target;
			criteria.minIndent = minIndent;
			criteria.strictlyCompare = strictlyCompare;
		}

		return criteria;
	}

} // anonymous namespace

TEST(SearchCriteriaTest, MakeFilter1)
{
	EXPECT_EQ(TextSearch::makeFilter("").size(), 0);
	EXPECT_EQ(TextSearch::makeFilter("*?").size(), 1);
	EXPECT_EQ(TextSearch::makeFilter("a*?").size(), 2);
	EXPECT_EQ(TextSearch::makeFilter("*?a*?").size(), 2);
	EXPECT_EQ(TextSearch::makeFilter("*?a*?a").size(), 2);
	EXPECT_EQ(TextSearch::makeFilter("a*?a*?").size(), 3);
	EXPECT_EQ(TextSearch::makeFilter("*?a*?a*?").size(), 3);
}

TEST(SearchCriteriaTest, MakeFilter2)
{
	TextSearch::Filter filter1 = TextSearch::makeFilter("*?*?*?*");
	TextSearch::Filter filter2 = TextSearch::makeFilter("???*");
	TextSearch::Filter filter3 = TextSearch::makeFilter("???");
	
	EXPECT_EQ(filter1.size(), 1);
	EXPECT_EQ(filter2.size(), 1);
	EXPECT_EQ(filter3.size(), 1);

	TextSearch::Criteria criteria1;
	EXPECT_TRUE(filter1.getNext(&criteria1));

	TextSearch::Criteria criteria2;
	EXPECT_TRUE(filter2.getNext(&criteria2));

	TextSearch::Criteria criteria3;
	EXPECT_TRUE(filter3.getNext(&criteria3));

	EXPECT_EQ(criteria1, criteria2);
	EXPECT_NE(criteria2, criteria3);
	
	EXPECT_FALSE(criteria2.strictlyCompare);
	EXPECT_TRUE(criteria3.strictlyCompare);
}

TEST(SearchCriteriaTest, CriteriaStrinctly)
{
	MyRange range;

	MyRange criteriaRange(2, 5);
	TextSearch::Criteria criteria = GetCriteria("abc", 2, true);

	EXPECT_TRUE(criteria.satisfies("ababcdef", range));
	EXPECT_EQ(range, criteriaRange);

	EXPECT_TRUE(criteria.satisfies("12abcabcabc", range));
	EXPECT_EQ(range, criteriaRange);

	EXPECT_FALSE(criteria.satisfies("1abcdef", range));
	EXPECT_FALSE(criteria.satisfies("123abcdef", range));
}

TEST(SearchCriteriaTest, CriteriaFloating)
{
	MyRange range;

	TextSearch::Criteria criteria = GetCriteria("abc", 2, false);

	EXPECT_TRUE(criteria.satisfies("ababcdef", range));
	EXPECT_EQ(range, MyRange(2, 5));

	EXPECT_TRUE(criteria.satisfies("1ababcdef", range));
	EXPECT_EQ(range, MyRange(3, 6));

	EXPECT_TRUE(criteria.satisfies("1234abcabcabc", range));
	EXPECT_EQ(range, MyRange(4, 7));

	EXPECT_FALSE(criteria.satisfies("1abcdef", range));
}

TEST(SearchCriteriaTest, FilterMatchPositive)
{
	EXPECT_TRUE(false);
}

TEST(SearchCriteriaTest, FilterMatchNegative)
{
	EXPECT_TRUE(false);
}