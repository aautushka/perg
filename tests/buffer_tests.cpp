#include <gtest/gtest.h>
#include "perg/buffer.h"

struct buffer_tests : public ::testing::Test
{
	using buffer = perg::buffer;
	buffer buf;
};

TEST_F(buffer_tests, creates_empty_buffer)
{
	EXPECT_EQ(0, buf.capacity());
	EXPECT_EQ(0, buf.size());
}

TEST_F(buffer_tests, adds_string)
{
	buf.copy("hello");
	EXPECT_EQ("hello", buf.convert<std::string>());

}

TEST_F(buffer_tests, adds_string_byte_by_byte)
{
	buf.copy("h");
	buf.copy("e");
	buf.copy("l");
	buf.copy("l");
	buf.copy("o");
	EXPECT_EQ("hello", buf.convert<std::string>());

}
