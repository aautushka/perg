#pragma once

#include <mutex>
#include "intrusive.h"

namespace perg
{

template <typename T>
class queue
{
public:
	explicit queue(size_t limit = std::numeric_limits<size_t>::max())
		: _size(0)
		, _limit(limit)
	{
	}

	void limit(size_t num)
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_limit = num;
	}

	void push(T t)
	{
		std::unique_lock<std::mutex> lock(_mutex);
		if (_size  < _limit)
		{
			_push_back(t);
		}			
		else
		{
			_size_changed.wait(lock, [this]{return this->_has_enough_space();});
			_push_back(t);
		}
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
		std::unique_lock<std::mutex> lock(_mutex);
		if (!_data.empty())
		{
			t = _data.pop_front();
			size_t prev = _size;
			--_size;
			_notify_blocked_thread(prev);
			return true;
		}

		return false;
	}

	perg::list<T> pop_all()
	{
		perg::list<T> out;
		{
			std::unique_lock<std::mutex> lock(_mutex);
			out = std::move(_data);

			size_t prev = _size;
			_size = 0;
			_notify_blocked_thread(prev);
		}
		return std::move(out);
	}

	size_t size()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		return _size;
	}

private:
	void _push_back(T t) {_data.push_back(t); ++_size; }
	bool _has_enough_space() { return _size < _limit; }

	void _notify_blocked_thread(size_t prevSize) 
	{
	       	if (prevSize >= _limit)
		{
			_size_changed.notify_one();
		}
	}

	size_t _size;
	size_t _limit;
	perg::list<T> _data;
	std::mutex _mutex;
	std::condition_variable _size_changed;

};

}

