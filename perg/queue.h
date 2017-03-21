#pragma once

#include <mutex>
#include "intrusive.h"

namespace perg
{

template <typename T>
class queue
{
public:
	queue()
		: _size(0)
	{
	}

	void push(T t)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		_data.push_back(t);
		++_size;
	}

	T pop()
	{
		T t;
		while (!try_pop(t))
		{
			// busy loop
		}

		return t;
	}

	bool try_pop(T& t)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (!_data.empty())
		{
			t = _data.pop_front();
			--_size;
			return true;
		}

		return false;
	}

	perg::list<T> pop_all()
	{
		perg::list<T> out;
		{
			std::lock_guard<std::mutex> lock(_mutex);
			out = std::move(_data);
			_size = 0;
		}
		return std::move(out);
	}

	size_t size()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return _size;
	}

private:
	size_t _size;
	perg::list<T> _data;
	std::mutex _mutex;

};

}

