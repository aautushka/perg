#pragma once

#include <thread>
#include <cassert>

#include "source.h"
#include "sink.h"
#include "proc.h"

namespace perg
{

template <typename T>
class pipeline
{
public:
	class connection
	{
	public:
		connection(pipeline<T>& p)
			: _pipe(p)
		{
		}

		connection& operator ()(proc<T>& p)
		{
			_pipe.connect(p);
			return *this;
		}

		connection& operator ()(sink<T>& s)
		{
			_pipe.connect(s);
			return *this;
		}

		connection& operator()(std::unique_ptr<proc<T>>& p)
		{
			return (*this)(*p);
		}

		connection& operator()(std::unique_ptr<sink<T>>& s)
		{
			return (*this)(*s);
		}

	private:
		pipeline<T>& _pipe;
	};

	connection connect(source<T>& input)
	{
		assert(_source == nullptr);
		
		_source = &input._channel;

		run_in_background(input);

		return connection(*this);
	}

	connection connect(std::unique_ptr<source<T>>& input)
	{
		return connect(*input);
	}

	~pipeline()
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

