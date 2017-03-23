#pragma once
#include <string.h>
#include <iostream>

namespace perg
{

// the class encapsulates char array
// no memory managment
// think of GSL span<T> 
class view
{
public:
	view(): _ptr(""), _size(0){}
	view(const char* str){assign(str);}
	view(const char* str, size_t size){assign(str, size);} 

	const char& operator [](int i) const
	{
		return _ptr[i];
	}

	size_t size() const
	{
		return _size;
	}

	bool empty() const
	{
		return size() == 0;
	}
	
	void grow()
	{
		_size += 1;
	}

	void assign(const char* str)
	{
		_ptr = str;
		_size = strlen(str);
	}

	void assign(const char* str, size_t size)
	{
		_ptr = str;
		_size = size;
	}
	
	bool operator ==(const view& other) const
	{
		if (empty() && other.empty())
		{
			return true;
		}
		else if (other._size == _size)
		{
			return 0 == memcmp(other._ptr, this->_ptr, this->_size); 
		}

		return false;
	}

	bool operator !=(const view& other) const
	{
		return !(*this == other);
	}

	const char* data() const
	{
		return _ptr;
	}

	void clear()
	{
		_ptr = "";
		_size = 0;
	}

	void erase(size_t size)
	{
		if (_size >= size)
		{
			_size -= size;
			_ptr += size;
		}
		else
		{
			clear();
		}
	}
	
private:
	const char* _ptr;
	size_t _size;
};

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

// look for a patter in the given buffer
// pattern may contain a star char (*) to match any number of random chars
// a question mark would match any single character 
bool glob_match(view buffer, view pattern);

view get_hint_from_mask(view mask);

std::ostream& operator <<(std::ostream& stream, const view& v);
} // namespace perg
