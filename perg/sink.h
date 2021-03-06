#pragma once

#include <iostream>
#include "channel.h"
#include "stage.h"

namespace perg
{

template <typename T>
class sink : public stage<T>
{
public:
	virtual ~sink() {}
	
private:
	void run()
	{
		while (_input->active())
		{
			process_one();
		}

		process_remaining();
	}

	// returns true if needs to continue processing
	// returns false to terminate immediately
	bool process_one()
	{
		auto tt = _input->read();

		for (T t : tt)
		{
			if (TERMINATE == this->process(t))
			{
				_input->close();
				return false;
			}

		}

		return !tt.empty();	
	}

	void process_remaining()
	{
		while (process_one());
	}

	bool connected()
	{
		return _input != nullptr;
	}

	channel<T>* _input = nullptr;

	template <typename U> friend class pipeline;

};
namespace sinks
{

template <typename T>
class collector : public perg::sink<T>
{
public:
	collector()
		: _limit(std::numeric_limits<size_t>::max())
	{
	}

	std::vector<T> result()
	{
		return _vec;
	}

	void limit(size_t maxNumOfCollectedItems)
	{
		_limit = maxNumOfCollectedItems;
	}

protected:
	virtual action process(T& t)
	{
		if (_vec.size() < _limit)
		{
			_vec.push_back(t);
			return UNDECIDED;
		}
		else
		{
			return TERMINATE;
		}
	}

private:
	std::vector<T> _vec;
	size_t _limit;
};

template <typename T>
class console : public perg::sink<T>
{
protected:
	virtual action process(T& t)
	{
		fwrite(t.data(), sizeof(char), t.size(), stdout);
		return UNDECIDED;
	}
};

template <typename T>
class null : public perg::sink<T>
{
protected:
	virtual action process(T& t)
	{
		return UNDECIDED;
	}
};
} // namespace sinks
} // namespace perg

