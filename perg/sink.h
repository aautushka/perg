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
namespace sinks
{

template <typename T>
class collector : public perg::sink<T>
{
public:
	std::vector<T> result()
	{
		return _vec;
	}

protected:
	virtual void process(T t)
	{
		_vec.push_back(t);
	}

private:
	std::vector<T> _vec;
};

template <typename T>
class console : public perg::sink<T>
{
protected:
	virtual void process(T t)
	{
		std::cout << t << std::endl;
	}
};
} // namespace sinks
} // namespace perg

