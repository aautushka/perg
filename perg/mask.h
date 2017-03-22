#pragma once
#include <string.h>
#include <iostream>

namespace perg
{
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

bool glob_match(view buffer, view pattern);
std::ostream& operator <<(std::ostream& stream, const view& v);
} // namespace perg
