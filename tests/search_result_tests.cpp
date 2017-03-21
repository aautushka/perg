#include <gtest/gtest.h>
#include "perg/pipe.h"
#include "perg/perg.h"

using test_source = perg::sources::generator<perg::view>;

struct search_result_tests : public ::testing::Test
{
	perg::pipeline<perg::view> pipeline;
	perg::search_result sink;
};

TEST_F(search_result_tests, gathers_search_result_in_a_buffer)
{
	test_source source = {"hello ", "world!", " how are you?"};

	pipeline.connect(source)(sink);
	pipeline.wait();

	perg::string_stream stream;
	sink.dump(stream);
	EXPECT_EQ("hello world! how are you?", stream.str); 
}

TEST_F(search_result_tests, define_custom_separator)
{
	test_source source = {"hello", "world!"};
	sink.separate_by(' ');

	pipeline.connect(source)(sink);
	pipeline.wait();

	perg::string_stream stream;
	sink.dump(stream);
	EXPECT_EQ("hello world! ", stream.str); 
}

TEST_F(search_result_tests, limits_the_number_of_items_in_the_resulting_buffer)
{
	test_source source = {"hello"};
	sink.limit(1);

	pipeline.connect(source)(sink);
	pipeline.wait();

	perg::string_stream stream;
	sink.dump(stream);
	EXPECT_EQ("hello", stream.str); 
}
