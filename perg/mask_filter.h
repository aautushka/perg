#pragma once
#include "proc.h"
#include "mask.h"

namespace perg
{
namespace filters
{
// checks if input matches the given mask
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

// breaks the input into lines and applies the filter
// reads_the_lines_in_foward_direction
struct forward_mask_filter : public perg::proc<perg::view>
{
public:
	explicit forward_mask_filter(const char* mask)
		: _mask(mask)
		, _ptr(nullptr)
	{
	}

protected:
	virtual action process(view& v)
	{
		if (v.data() != _prev.data())
		{
			_prev = v;
			_ptr = v.data();

		}

		const char* end = v.data() + v.size();
		if (_ptr < end)
		{
			const char* found = (const char*)memmem(_ptr, end - _ptr, _mask.data(), _mask.size());	
			if (found)
			{
				const char* beg = v.data();

				const char* nextline = (const char*)memchr(found, '\n', end - found);		
				nextline = nextline ? nextline : end;

				const char* prevline = (const char*)memrchr(beg, '\n', found - beg);
				prevline = prevline ? prevline + 1 : beg;
				v.assign(prevline, nextline - prevline);
				_ptr = nextline + 1;
				return PASS_DOWNSTREAM_AND_REPEAT; 
			}
		}
		return FILTER_OUT;
	}	
	
private:
	view _mask;
	view _prev;
	const char* _ptr;

};

inline void* memrmem(const void* haystack, size_t haystacklen, const void* needlePtr, size_t needleLen)
{
	if (needleLen > haystacklen)
	{
		return nullptr;
	}

	const unsigned char* beg = (const unsigned char*)haystack;
	const unsigned char* end = beg + haystacklen;
	const unsigned char* cur = end - needleLen + 1;
	const unsigned char* needle = (const unsigned char*) needlePtr;

	while (true)
	{
		const unsigned char* found = (const unsigned char*)memrchr(beg, needle[0], cur - beg);
		if (found)
		{
			if (0 == memcmp(found, needle, needleLen))
			{
				return (void*)found;
			}
			cur = found - 1;
		}
		else
		{
			break;
		}
	}
	return nullptr;
}

// breaks the input into lines and applies the filter
// reads_the_lines_in_foward_direction
struct backward_mask_filter : public perg::proc<perg::view>
{
public:
	explicit backward_mask_filter(const char* mask)
		: _mask(mask)
		, _ptr(nullptr)
	{
	}

protected:
	virtual action process(view& v)
	{
		if (v.data() != _prev.data())
		{
			_prev = v;
			_ptr = v.data() + v.size();
		}

		const char* beg = v.data();
		const char* end = v.data() + v.size();
		if (_ptr > beg)
		{
			const char* found = (const char*)memrmem(beg, _ptr - beg, _mask.data(), _mask.size());	
			if (found)
			{
				const char* nextline = (const char*)memchr(found, '\n', end - found);		
				nextline = nextline ? nextline : end;

				const char* prevline = (const char*)memrchr(beg, '\n', found - beg);
				prevline = prevline ? prevline + 1 : beg;
				v.assign(prevline, nextline - prevline);
				_ptr = prevline - 1;
				return PASS_DOWNSTREAM_AND_REPEAT; 
			}
		}
		return FILTER_OUT;
	}	
	
private:
	view _mask;
	view _prev;
	const char* _ptr;

};

} // namespace filters
} // namespace perg

