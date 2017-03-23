#include "mask.h"
#include <cstring>
#include <cassert>

namespace perg
{

class glob_haystack
{
public:
	explicit glob_haystack(view buffer)
		: _buffer(buffer)
	{
	}

	bool move(size_t off)
	{
		if (_buffer.size() >= off)
		{
			_buffer.erase(off);
			return true;
		}
		_buffer.clear();
		return false;
	}
	
	bool move_past(view str)
	{
		if (str.size() > 0)
		{
			void* found = memmem(_buffer.data(), _buffer.size(), str.data(), str.size());
			if (found != nullptr)
			{
				size_t offset = str.size() + static_cast<const char*>(found) - _buffer.data();
				_buffer.erase(offset);
				return true;
			}
			_buffer.clear();
			return false;
		}

		return true;
	}


private:
	view _buffer;
};

bool glob_match(view buffer, view rawPattern)
{

	if (!buffer.empty() && !rawPattern.empty())
	{
		glob_pattern pattern(rawPattern); 	
		glob_haystack haystack(buffer);
		
		while (glob_state state = pattern.walk())
		{
			switch (state)
			{
			case STAR:
				break;
			case QUESTION:
				if (!haystack.move(1))
				{
					return false;
				}
				break;
			case TEXT:
				if (!haystack.move_past(pattern.read()))
				{
					return false;
				}
				break;
			default:
				assert(false);
			}
		}
		return true;
	}

	return rawPattern.empty();
}

std::ostream& operator <<(std::ostream& stream, const view& v)
{
	return stream.write(v.data(), v.size());
}

view get_hint_from_mask(view mask)
{
	view hint;
	glob_pattern pattern(mask);
	glob_state state = UNKNOWN;
	do
	{
		auto state = pattern.walk();
		if (state == TEXT)
		{
			view chunk = pattern.read();
			if (chunk.size() > hint.size())
			{
				hint = chunk;
			}
		}
	}
	while (state != UNKNOWN);

	return hint;
}


} // namespace perg

