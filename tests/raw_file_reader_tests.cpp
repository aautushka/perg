#include <gtest/gtest.h>
#include "perg/perg.h"
#include "temp_file.h"

struct raw_file_reader_tests : public ::testing::Test
{
	perg::pipeline<perg::view> pipeline;
	perg::sinks::collector<perg::view> sink;
	perg::raw_file_reader source;
	tests::temp_file file;

	using result = std::vector<perg::view>;

	raw_file_reader_tests()
	{
	}

	void run_pipeline()
	{
		source.open(file.name());
		pipeline.connect(source)(sink);
		pipeline.wait();
	}

	~raw_file_reader_tests()
	{
	}
};

TEST_F(raw_file_reader_tests, reads_whole_file)
{
	file.write_line("hello world");
	file.write_line("how are you");

	run_pipeline();
	
	result expected = {"hello world\nhow are you\n"}; 
	EXPECT_EQ(expected, sink.result());
	
}

