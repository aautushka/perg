#include <gtest/gtest.h>
#include "perg/pipe.h"

using test_source = perg::sources::generator<int>;
using test_sink = perg::sinks::collector<int>;

struct pipeline_test : public ::testing::Test
{
	perg::mypipeline<int> pipeline;
};

TEST_F(pipeline_test, connects_source_to_sink_directly)
{
	test_source source = {1, 2, 3};
	test_sink sink; 

	pipeline.connect(source)(sink);
	pipeline.wait();

	std::vector<int> expected = {1, 2, 3};
	EXPECT_EQ(expected, sink.result());
}

TEST_F(pipeline_test, calls_proc_to_filter_even_numbers)
{
	test_source source = {1, 2, 3, 4, 5};
	auto filter = perg::make_filter<int>([](int& i) { return i % 2 ? true : false;});
	test_sink sink; 

	pipeline.connect(source)(*filter)(sink);
	pipeline.wait();

	std::vector<int> expected = {1, 3, 5};
	EXPECT_EQ(expected, sink.result());
}

