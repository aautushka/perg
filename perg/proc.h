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

private:
	void run()
	{
		while (_input->active()) 
		{
			process_one();
		}

		process_remaining();
		_output.close();
	}

	bool process_one()
	{
		T t;

		if (_input->try_read(t))
		{
			action act = this->process(t);
			if (act == PASS_DOWNSTREAM)
			{
				_output.write(t);
			}
			return act !=  TERMINATE;
		}

		return false;
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
	channel<T> _output;

	template <typename U> friend class pipeline;
};

template <typename T, typename Func> 
std::unique_ptr<proc<T>> make_filter(Func func)
{
	struct proc_type : public proc<T>
	{
		proc_type(Func func) : func(func) {}
		virtual action process(T& t) {return func(t);}
		Func func;
	};

	auto ptr = new proc_type(func);
	return std::unique_ptr<proc<T>>(ptr);
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

} // namespace filters

} // namespce perg

