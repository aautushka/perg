#pragma once

#include <atomic>
#include <memory>
#include "queue.h"

namespace perg
{

template <typename T>
class channel
{
public:
	channel()
	{
		_active = true;
		_queue.limit(10000);
	}

	channel(channel&& other)
	{
		*this = std::move(other);
	}

	channel& operator =(channel&& other)
	{
		_queue = std::move(other._queue);
		_active = other._active.load();
		other._active = false;
		return *this;
	}

	void write(T t)
	{
		_queue.push(t);
	}

	void write(list<T>&& tt)
	{
		_queue.push(std::move(tt));
	}

	perg::list<T> read()
	{
		return _queue.pop_all();
	}

	bool try_read(T& t)
	{
		return _queue.try_pop(t);
	}
	
	void close()
	{
		_active = false;
	}
	
	bool active()
	{
		return _active;
	}

private:
	queue<T> _queue;
	std::atomic<bool> _active;
};

}

