#pragma once

#include <vector>
#include <iostream>
#include "channel.h"
#include "stage.h"

namespace perg
{

template <typename T>
class source : public stage<T>
{
public:
	virtual ~source() {}

private:
	void run()
	{
		using batch_t = typename channel<T>::batch_t;
		T t;
		batch_t buffer;
		size_t processed = 0;

		action act = UNDECIDED; 
		while (act != TERMINATE && _channel.active())
		{
			act = this->process(t);
			if (act == PASS_DOWNSTREAM)
			{
				buffer.push_back(t);

				if (++processed % 100 == 0)
				{
					_channel.write(std::move(buffer));		
				}
			}
		}

		_channel.write(std::move(buffer));		
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
	virtual action process(T& i)
	{
		if (_pos < _vec.size())
		{
			i = _vec[_pos++];
			return PASS_DOWNSTREAM;
		}

		return TERMINATE;
	}

private:
	std::vector<T> _vec;
	size_t _pos = 0;
};
} // namespace sources

}

