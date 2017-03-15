#include <gtest/gtest.h>
#include "perg/pipe.h"

class test_source : public perg::source<int>
{
public: 
	test_source(std::initializer_list<int> list)
		: _vec(std::move(list))
	{
	}

protected:
	virtual bool process(int& i)
	{
		if (_pos < _vec.size())
		{
			i = _vec[_pos++];
			return true;
		}

		return false;
	}

private:
	std::vector<int> _vec;
	size_t _pos = 0;
};

class test_sink : public perg::sink<int>
{
public:
	std::vector<int> result()
	{
		return _vec;
	}

protected:
	virtual void process(int i)
	{
		_vec.push_back(i);
	}

private:
	std::vector<int> _vec;
};

struct pipeline_test : public ::testing::Test
{
	perg::pipe<int> pipeline;
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

