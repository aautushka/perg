#pragma once

#include <vector>
#include "channel.h"

namespace perg
{

template <typename T>
class source
{
public:
	virtual ~source() {}

protected:
	virtual bool process(T& t) = 0;

private:
	void run()
	{
		T t;
		while (process(t))
		{
			_channel.write(t);
		}

		_channel.close();
	}

	channel<T> _channel;

	template <typename U> friend class pipeline;
};


namespace sources
{
template <typename T> 
class generator : public perg::source<T>
{
public: 
	generator(std::initializer_list<T> list)
		: _vec(std::move(list))
	{
	}

protected:
	virtual bool process(T& i)
	{
		if (_pos < _vec.size())
		{
			i = _vec[_pos++];
			return true;
		}

		return false;
	}

private:
	std::vector<T> _vec;
	size_t _pos = 0;
};
} // namespace sources

}

