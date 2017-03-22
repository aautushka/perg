#pragma once

#include <atomic>
#include <memory>
#include "queue.h"

namespace perg
{

// communication channel used to pass messages between threads
template <typename T>
class channel
{
public:
	using batch_t = typename queue<T>::batch_t;

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

	void write(batch_t&& tt)
	{
		_queue.push(std::move(tt));
	}

	batch_t read()
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

