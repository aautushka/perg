#pragma once

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

	template <typename U> friend class pipe;
};

}

