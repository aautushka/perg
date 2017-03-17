#pragma once
#include "pipe.h"
#include "buffer.h"
#include "mask_filter.h"

namespace perg
{
class search_result : public sink<view>
{
public:
	void dump(stream& ss)
	{
		_buffer.dump(ss);
	}

protected:
	virtual void process(view v)
	{
		_buffer.copy(v.data(), v.size());
	}

private:
	buffer _buffer;
};
} //namespace perg


