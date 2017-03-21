#pragma once

namespace perg
{
enum action
{
	UNDECIDED,
	PASS_DOWNSTREAM,
	FILTER_OUT,
	TERMINATE
};

template <typename T>
class stage
{
	protected:
		virtual ~stage() {}
		virtual action process(T& t) = 0;
};
		
} // namespace perg

