#include <gtest/gtest.h>
#include "perg/perg.h"
#include "temp_file.h"

struct line_reader_tests : public ::testing::Test
{
	perg::pipeline<perg::view> pipeline;
	perg::sinks::collector<perg::view> sink;
	perg::line_reader source;
	tests::temp_file file;

	using result = std::vector<perg::view>;

	line_reader_tests()
	{
	}

	void run_pipeline()
	{
		source.open(file.name());
		pipeline.connect(source)(sink);
		pipeline.wait();
	}

	~line_reader_tests()
	{
	}
};
TEST_F(line_reader_tests, reads_a_file_with_one_line_only)
{
	file.write_line("hello world");

	run_pipeline();
	
	result expected = {"hello world"}; 
	EXPECT_EQ(expected, sink.result());
	
}

TEST_F(line_reader_tests, reads_multiple_lines)
{
	file.write_line("hello world");
	file.write_line("how are you");

	run_pipeline();
	
	result expected = {"hello world", "how are you"}; 
	EXPECT_EQ(expected, sink.result());
	
}

TEST_F(line_reader_tests, reads_whole_file_if_there_are_no_newlines)
{
	file.write("hello world");

	run_pipeline();
	
	result expected = {"hello world"}; 
	EXPECT_EQ(expected, sink.result());
	
}

TEST_F(line_reader_tests, reads_empty_line)
{
	file.write("\n");

	run_pipeline();
	
	result expected = {""}; 
	EXPECT_EQ(expected, sink.result());
	
}
