#include <gtest/gtest.h>
#include "perg/pipe.h"
#include "perg/perg.h"

using test_source = perg::sources::generator<perg::view>;

struct search_result_tests : public ::testing::Test
{
	perg::mypipeline<perg::view> pipeline;
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
