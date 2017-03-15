#pragma once

#include "channel.h"

namespace perg
{

template <typename T>
class sink
{
public:
	virtual ~sink() {}
protected:
	virtual void process(T t) = 0;

private:
	void run()
	{
		while (_input->active())
		{
			process_one();
		}

		flush();
	}

	void process_one()
	{
		T t;

		if (_input->try_read(t))
		{
			process(t);
		}
	}

	void flush()
	{
		T t;

		while (_input->try_read(t))
		{
			process(t);
		}
	}

	bool connected()
	{
		return _input != nullptr;
	}

	channel<T>* _input = nullptr;

	template <typename U> friend class pipe;

};

}

