#include <gtest/gtest.h>
#include "perg/pipe.h"
#include "perg/mask_filter.h"

using test_source = perg::sources::generator<perg::view>;
using test_sink = perg::sinks::collector<perg::view>;

struct mask_filter_test : public ::testing::Test
{
	using result = std::vector<perg::view>;
	perg::pipeline<perg::view> pipeline;
};

TEST_F(mask_filter_test, filter_out_strings_which_do_not_match_the_given_mask)
{
	test_source source = {"hello world", "how are you", "hope you're fine"};
	test_sink sink; 
	perg::filters::mask_filter filter("you");

	pipeline.connect(source)(filter)(sink);
	pipeline.wait();

	result expected = {"how are you", "hope you're fine"};
	EXPECT_EQ(expected, sink.result());
}
