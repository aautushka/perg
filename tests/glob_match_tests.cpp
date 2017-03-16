#include <gtest/gtest.h>
#include "perg/mask.h"

struct glob_match_text : public ::testing::Test
{
};

TEST_F(glob_match_text, matches_everything)
{
	EXPECT_TRUE(perg::glob_match("hello world", "*"));
}

TEST_F(glob_match_text, empty_pattern_matches_everything)
{
	EXPECT_TRUE(perg::glob_match("hello world", ""));
}

TEST_F(glob_match_text, fails_to_find_a_substring)
{
	EXPECT_FALSE(perg::glob_match("hello world", "beautiful"));
}

TEST_F(glob_match_text, matches_complete_string)
{
	EXPECT_TRUE(perg::glob_match("hello", "hello"));
}

TEST_F(glob_match_text, matches_substring_in_the_beginning)
{
	EXPECT_TRUE(perg::glob_match("hello world", "hello"));
}

TEST_F(glob_match_text, matches_substring_at_the_end)
{
	EXPECT_TRUE(perg::glob_match("hello world", "world"));
}

TEST_F(glob_match_text, matches_pattern_with_a_question_mark_in_the_middle)
{
	EXPECT_TRUE(perg::glob_match("hello", "he?lo"));
}

TEST_F(glob_match_text, matches_patter_with_a_question_mark_in_the_beginning)
{
	EXPECT_TRUE(perg::glob_match("hello", "?ello"));
}

TEST_F(glob_match_text, matches_pattern_with_a_question_mar_in_the_middle)
{
	EXPECT_TRUE(perg::glob_match("hello", "hell?"));
}

TEST_F(glob_match_text, matches_substring)
{
	EXPECT_TRUE(perg::glob_match("hello beautiful world", "beautiful"));
}

TEST_F(glob_match_text, matches_pattern_with_multiple_stars)
{
	EXPECT_TRUE(perg::glob_match("hello beautiful world", "hell*bea*wor*"));
}

TEST_F(glob_match_text, a_star_may_not_match_anything)
{
	EXPECT_TRUE(perg::glob_match("hello world", "hello world*"));
}

TEST_F(glob_match_text, a_question_mark_is_required_to_match_a_character)
{
	EXPECT_FALSE(perg::glob_match("hello world", "hello world?"));
}

TEST_F(glob_match_text, nothing_matches_empty_string)
{
	EXPECT_FALSE(perg::glob_match("", "hello"));
}

TEST_F(glob_match_text, only_a_star_may_match_empty_string)
{
	EXPECT_FALSE(perg::glob_match("", "*"));
}
