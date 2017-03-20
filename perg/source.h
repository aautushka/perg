#pragma once

#include <vector>
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
		T t;
		action act = UNDECIDED; 
		while (act != TERMINATE)
		{
			act = this->process(t);
			if (act == PASS_DOWNSTREAM)
			{
				_channel.write(t);
			}
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

