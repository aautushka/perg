#pragma once
#include <cstring>

namespace perg
{
class view
{
public:
	view(): _ptr(""), _size(0){}
	view(const char* str): _ptr(str), _size(strlen(str)) { }
	view(const char* str, size_t size): _ptr(str), _size(size) {} 

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
		if (!empty() && other._size == _size)
		{
			return memcmp(other._ptr, this->_ptr, this->_size); 
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

bool glob_match(view buffer, view pattern);
} // namespace perg
