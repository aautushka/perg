#include <gtest/gtest.h>
#include <stdlib.h>
#include "perg/perg.h"
#include "temp_file.h"

struct rfile_reader_tests : public ::testing::Test
{
	perg::pipeline<perg::view> pipeline;
	perg::sinks::collector<perg::view> sink;
	perg::reverse_file_reader source;
	tests::temp_file file;

	using result = std::vector<perg::view>;

	rfile_reader_tests()
	{
	}

	void run_pipeline()
	{
		source.open(file.name());
		pipeline.connect(source)(sink);
		pipeline.wait();
	}

	~rfile_reader_tests()
	{
	}
};

TEST_F(rfile_reader_tests, reads_a_file_with_one_line_only)
{
	file.write_line("hello world");

	run_pipeline();
	
	result expected = {"hello world"}; 
	EXPECT_EQ(expected, sink.result());
	
}

TEST_F(rfile_reader_tests, reads_multiple_lines_in_reverse_order)
{
	file.write_line("hello world");
	file.write_line("how are you");

	run_pipeline();
	
	result expected = {"how are you", "hello world"};
	EXPECT_EQ(expected, sink.result());
	
}

TEST_F(rfile_reader_tests, reads_whole_file_if_there_are_no_newlines)
{
	file.write("hello world");

	run_pipeline();
	
	result expected = {"hello world"}; 
	EXPECT_EQ(expected, sink.result());
	
}

TEST_F(rfile_reader_tests, reads_empty_line)
{
	file.write("\n");

	run_pipeline();
	
	result expected = {""}; 
	EXPECT_EQ(expected, sink.result());
	
}
