#include <gtest/gtest.h>
#include <stdlib.h>
#include "perg/perg.h"

class temp_file
{
public:
	temp_file()
	{
		char name[] = "/tmp/perg-utests-XXXXXX";
		_file_desc = mkstemp(name);
		_file_name.assign(name);
	}

	~temp_file()
	{
		close(_file_desc);
	}

	void write_line(const char* str)
	{
		::write(_file_desc, str, strlen(str));
		::write(_file_desc, "\n", 1);
		fsync(_file_desc);
	}

	void write(const char* str)
	{
		::write(_file_desc, str, strlen(str));
		fsync(_file_desc);
	}

	const char* name() const
	{
		return _file_name.c_str();
	}
private:
	int _file_desc;
	std::string _file_name;
};

struct file_reader_tests : public ::testing::Test
{
	perg::mypipeline<perg::view> pipeline;
	perg::sinks::collector<perg::view> sink;
	perg::file_reader source;
	temp_file file;

	using result = std::vector<perg::view>;

	file_reader_tests()
	{
	}

	void run_pipeline()
	{
		source.open(file.name());
		pipeline.connect(source)(sink);
		pipeline.wait();
	}

	~file_reader_tests()
	{
	}
};
TEST_F(file_reader_tests, reads_a_file_with_one_line_only)
{
	file.write_line("hello world");

	run_pipeline();
	
	result expected = {"hello world\n"}; 
	EXPECT_EQ(expected, sink.result());
	
}

TEST_F(file_reader_tests, reads_multiple_lines)
{
	file.write_line("hello world");
	file.write_line("how are you");

	run_pipeline();
	
	result expected = {"hello world\n", "how are you\n"}; 
	EXPECT_EQ(expected, sink.result());
	
}

TEST_F(file_reader_tests, reads_whole_file_if_there_are_no_newlines)
{
	file.write_line("hello world");

	run_pipeline();
	
	result expected = {"hello world\n"}; 
	EXPECT_EQ(expected, sink.result());
	
}
