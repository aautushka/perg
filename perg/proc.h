#pragma once

#include "channel.h"
#include "pipe.h"
#include "stage.h"

namespace perg
{

template <typename T>
class proc : public stage<T>
{
public:
	virtual ~proc() {}
	proc(){ }

	proc(proc&& other)
	{
		*this = std::move(other);
	}

	proc& operator =(proc&& other)
	{
		_output = std::move(other._output);
		_input = other._input;
		other._input = nullptr;
		return *this;
	}

private:
	void run()
	{
		while (_input->active() && _output.active()) 
		{
			if (TERMINATE == process_one())
			{
				break;
			}
		}

		process_remaining();
		_output.close();
		_input->close();
	}

	action process_one()
	{
		auto tt = _input->read();
		action act = UNDECIDED;

		for (T t : tt)
		{
			T processed = t;
			act = this->process(processed);
			if (act == PASS_DOWNSTREAM)
			{
				_output.write(processed);
				continue;
			}
			else if (act == PASS_DOWNSTREAM_AND_REPEAT)
			{
				while (act == PASS_DOWNSTREAM_AND_REPEAT)
				{
					_output.write(processed);
					processed = t;
					act = this->process(processed);
				}
			} 
			
			if (act == TERMINATE)
			{
				break;
			}
		}

		return act;
	}

	void process_remaining()
	{
		process_one();
	}

	bool connected()
	{
		return _input != nullptr;
	}

	channel<T>* _input = nullptr;
	channel<T> _output;

	template <typename U> friend class pipeline;
};


namespace filters
{
template <typename T>
class pass_thru : public proc<T>
{
protected:
	virtual action process(T& t) { return PASS_DOWNSTREAM; }
};

template <typename T>
class block : public proc<T>
{
protected:
	virtual action process(T& t) { return FILTER_OUT; }
};

template <typename T, typename Func>
class filter : public proc<T>
{
public:
	explicit filter(Func func) : func(func) {}
protected:
	virtual action process(T& t) {return func(t);}
private:
	Func func;
};

template <typename T, typename Func> 
filter<T, Func> make_filter(Func func)
{

	filter<T, Func> filter(func);
	return std::move(filter);
};
} // namespace filters

} // namespce perg

