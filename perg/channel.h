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
	}

	void write(T t)
	{
		_queue.push(t);
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

