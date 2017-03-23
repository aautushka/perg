#include <gtest/gtest.h>
#include "perg/pipe.h"
#include "perg/mask_filter.h"

using test_source = perg::sources::generator<perg::view>;
using test_sink = perg::sinks::collector<perg::view>;

struct forward_mask_filter_test : public ::testing::Test
{
	using result = std::vector<perg::view>;
	perg::pipeline<perg::view> pipeline;
};

TEST_F(forward_mask_filter_test, finds_a_matching_line_in_input_stream)
{
	test_source source = {"hello world\nhow are you\ngood day"};
	test_sink sink; 
	perg::filters::forward_mask_filter filter("you");

	pipeline.connect(source)(filter)(sink);
	pipeline.wait();

	result expected = {"how are you"};
	EXPECT_EQ(expected, sink.result());
}

TEST_F(forward_mask_filter_test, finds_a_matching_stream_at_the_beginning)
{
	test_source source = {"hello world\nhow are you\ngood day"};
	test_sink sink; 
	perg::filters::forward_mask_filter filter("world");

	pipeline.connect(source)(filter)(sink);
	pipeline.wait();

	result expected = {"hello world"};
	EXPECT_EQ(expected, sink.result());
}

TEST_F(forward_mask_filter_test, finds_a_match_at_the_end)
{
	test_source source = {"hello world\nhow are you\ngood day"};
	test_sink sink; 
	perg::filters::forward_mask_filter filter("good");

	pipeline.connect(source)(filter)(sink);
	pipeline.wait();

	result expected = {"good day"};
	EXPECT_EQ(expected, sink.result());
}
