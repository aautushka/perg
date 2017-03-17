#pragma once

#include <thread>

#include "source.h"
#include "sink.h"
#include "proc.h"

namespace perg
{

template <typename T>
class mypipeline
{
public:
	class connection
	{
	public:
		connection(mypipeline<T>& p)
			: _pipe(p)
		{
		}

		connection& operator ()(proc<T>& p)
		{
			_pipe.connect(p);
			return *this;
		}

		connection & operator ()(sink<T>& s)
		{
			_pipe.connect(s);
			return *this;
		}

	private:
		mypipeline<T>& _pipe;
	};

	connection connect(source<T>& input)
	{
		assert(_source == nullptr);
		
		_source = &input._channel;

		run_in_background(input);

		return connection(*this);
	}

	~mypipeline()
	{
		wait();
	}

	void wait()
	{
		for (auto& t: _threads)
		{
			t.join();
		}

		_threads.clear();
	}

private:

	template <typename U>
	void run_in_background(U& u)
	{
		_threads.push_back(std::thread([&u](){u.run();}));
	}

	void connect(proc<T>& filter)
	{
		assert(!filter.connected());
		assert(_source != nullptr);

		filter._input = _source;
		_source = &filter._output;

		run_in_background(filter);
	}

	void connect(sink<T>& output)
	{
		assert(!output.connected());
		assert(_source != nullptr);

		output._input = _source;
		_source = nullptr;

		run_in_background(output);
	}

	std::vector<std::thread> _threads;
	channel<T>* _source = nullptr;

};

}

