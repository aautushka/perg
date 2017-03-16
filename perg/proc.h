#pragma once

#include "channel.h"

namespace perg
{

template <typename T>
class proc
{
public:
	virtual ~proc() {}

protected:
	virtual bool process(T& t) = 0;

private:
	void run()
	{
		while (_input->active())
		{
			process_one();
		}

		flush();

		_output.close();
	}

	bool process_one()
	{
		T t;

		if (_input->try_read(t))
		{
			if (process(t))
			{
				_output.write(t);
			}
			return true;
		}

		return false;
	}

	void flush()
	{
		while (process_one());
	}

	bool connected()
	{
		return _input != nullptr;
	}

	channel<T>* _input = nullptr;
	channel<T> _output;

	template <typename U> friend class pipe;
};

template <typename T, typename Func> 
std::unique_ptr<proc<T>> make_filter(Func func)
{
	struct proc_type : public proc<T>
	{
		proc_type(Func func) : func(func) {}
		virtual bool process(T& t) {return func(t);}
		Func func;
	};

	auto ptr = new proc_type(func);
	return std::unique_ptr<proc<T>>(ptr);
};

namespace filters
{
} // namespace filters

} // namespce perg

