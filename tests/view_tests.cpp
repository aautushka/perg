#include <gtest/gtest.h>
#include "perg/mask.h"

struct view_tests : public ::testing::Test
{
};

using perg::view;

TEST_F(view_tests, compares_equal_views)
{
	EXPECT_TRUE(view("hello") == view("hello"));
}

TEST_F(view_tests, views_are_not_equal)
{
	EXPECT_FALSE(view("hello") == view("world"));
}

TEST_F(view_tests, views_of_different_sizes_cant_be_equal)
{
	EXPECT_FALSE(view("hello") == view("world!"));
}

