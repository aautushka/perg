#include "mask.h"
#include <cstring>
#include <cassert>

namespace perg
{

enum glob_state
{
	UNKNOWN = 0,
	STAR,
	QUESTION,
	TEXT	
};

class glob_pattern
{
public:

	explicit glob_pattern(view pattern)
		: _pattern(pattern)
		, _pos(0)
		, _current_state(UNKNOWN)
	{
	}

	glob_state walk()
	{
		_current_state = UNKNOWN;
		while (more())
		{
			const char ch = get();
			switch (ch)
			{
			case '*':
				if (_current_state == UNKNOWN)
				{
					_current_state = STAR;
					_current_chunk.assign(&_pattern[_pos], 1);
					next();
				}
				return _current_state;
			case '?':
				if (_current_state == UNKNOWN)
				{
					_current_state = QUESTION;
					_current_chunk.assign(&_pattern[_pos], 1);
					next();
				}
				return _current_state;
			default: 
				if (_current_state == TEXT)
				{
					_current_chunk.grow();
				}
				else
				{
					_current_chunk.assign(&_pattern[_pos], 1);
					_current_state = TEXT; 
				}
			}
			next();
		}
		return _current_state;
	}

	view read()
	{
		return _current_chunk;
	}
	
private:
	bool more()
	{
		return _pos < _pattern.size();
	}

	char get()
	{
		return _pattern[_pos];
	}

	void next()
	{
		++_pos;
	}

	view _pattern;

	size_t _pos;
	glob_state _current_state;
	view _current_chunk;
};

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

} // namespace perg

