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

TEST_F(buffer_tests, count_number_of_bytes_written)
{
	buf.copy("hello");
	EXPECT_EQ(5, buf.size());
}


TEST_F(buffer_tests, allocates_a_little_more_than_needed_for_optimization_purposes) {
	buf.copy("h");
	buf.copy("e");
	EXPECT_GT(buf.capacity(), buf.size());
}

TEST_F(buffer_tests, writes_buffer_to_stream)
{
 	buf.copy("hello ");
	buf.copy("world");

	perg::string_stream stream;	
	buf.dump(stream);
 	EXPECT_EQ("hello world", stream.str); 
}
 
