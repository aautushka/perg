#pragma once

#include <list>
#include <mutex>

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
			t = _data.front();
			_data.pop_front();
			return true;
		}

		return false;
	}

private:
	std::list<T> _data;
	std::mutex _mutex;

};

}

