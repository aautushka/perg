#pragma once

#include <mutex>
#include <condition_variable>
#include <chrono>
#include "intrusive.h"
#include "block_list.h"

namespace perg
{

// multithreaded queue used for passing messages between thread
// in essense it's a list protected by a mutex
// might have an upper limit
// when full, block untils the reader thread frees some space
template <typename T>
class queue
{
public:
	using batch_t = block_list<T, 512>;
	//using batch_t = list<T>;

	explicit queue(size_t limit = std::numeric_limits<size_t>::max())
		: _size(0)
		, _limit(limit)
	{
	}

	queue(queue&& other)
	{
		*this = std::move(other);
	}

	queue& operator =(queue&& other)
	{
		_size = other._size;
		_limit = other._limit;
		_data = std::move(other._data);
		other._size = 0;
		return * this;
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
			using namespace std::chrono_literals;
			_size_changed.wait_for(lock, 100ms, [this]{return this->_has_enough_space();});
			_push_back(t);
		}
	}


	void push(batch_t&& tt)
	{

		std::unique_lock<std::mutex> lock(_mutex);
		if (_size  < _limit)
		{
			_push_back(std::move(tt));
		}			
		else
		{
			_size_changed.wait(lock, [this]{return this->_has_enough_space();});
			_push_back(std::move(tt));
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

	batch_t pop_all()
	{
		batch_t out;
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
	
	void _push_back(batch_t&& tt) 
	{ 
		_size += tt.size();
	       	_data.push_back(std::move(tt)); 
	}

	void _notify_blocked_thread(size_t prevSize) 
	{
	       	if (prevSize >= _limit)
		{
			_size_changed.notify_one();
		}
	}

	size_t _size;
	size_t _limit;
	batch_t _data;
	std::mutex _mutex;
	std::condition_variable _size_changed;

};

}

