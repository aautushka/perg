#pragma once
#include "proc.h"
#include "mask.h"

namespace perg
{
namespace filters
{
struct mask_filter : public perg::proc<perg::view>
{
public:
	explicit mask_filter(const char* mask)
		: _mask(mask)
	{
	}

protected:
	virtual action process(view& v)
	{
		return glob_match(v, _mask) ? PASS_DOWNSTREAM : FILTER_OUT;
	}	
	
private:
	view _mask;

};

} // namespace filters
} // namespace perg
