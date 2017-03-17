#pragma once

#include <mutex>
#include "intrusive.h"

namespace perg
{

template <typename T>
class queue
{
public:
	void push(T t)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		_data.push_back(t);
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
			return true;
		}

		return false;
	}

private:
	perg::list<T> _data;
	std::mutex _mutex;

};

}

