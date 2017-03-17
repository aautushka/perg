#pragma once
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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

class file_reader : public source<view>
{
public:
	file_reader()
		: _file_desc(-1)
		, _file_ptr(nullptr)
		, _file_end(nullptr)
		, _cur_ptr(nullptr)
		, _file_size(0)
	{
	}

	explicit file_reader(const char* filename)
		: _file_desc(-1)
		, _file_ptr(nullptr)
		, _file_end(nullptr)
		, _cur_ptr(nullptr)
		, _file_size(0)
	{
		open(filename);
	}

	void open(const char* filename)
	{
		assert(_file_desc == -1);
		int _file_desc = ::open(filename, O_RDONLY);
		if (_file_desc != -1)
		{
			struct stat sb;

			if (fstat(_file_desc, &sb) != -1)
			{
				_file_size = sb.st_size;
				_file_ptr = (char*)mmap(0, _file_size, PROT_READ, MAP_SHARED, _file_desc, 0);
				_file_end = _file_ptr + _file_size;
				_cur_ptr = _file_ptr;
			}
		}
	}

	~file_reader()
	{
		if (_file_ptr)
		{
			close(_file_desc);
			munmap(_file_ptr, _file_size);
		}
	}

protected:
	virtual bool process(view& v)
	{
		if (_cur_ptr != _file_end)
		{
			char* nextLine = (char*)memchr(_cur_ptr, '\n', _file_end - _cur_ptr);
			if (nextLine)
			{
				nextLine += 1;
			}
			else
			{
				nextLine = _file_end;
			}
			
			v.assign(_cur_ptr, nextLine - _cur_ptr);
			_cur_ptr = nextLine;
			
			return true;
		}
		return false;
	}
private:
	int _file_desc;
	char* _file_ptr;
	char* _file_end;
	char* _cur_ptr;
	size_t _file_size;

};

class reverse_file_reader : public source<view>
{
public:
	reverse_file_reader()
		: _file_desc(-1)
		, _file_ptr(nullptr)
		, _file_end(nullptr)
		, _cur_ptr(nullptr)
		, _file_size(0)
	{
	}

	explicit reverse_file_reader(const char* filename)
		: _file_desc(-1)
		, _file_ptr(nullptr)
		, _file_end(nullptr)
		, _cur_ptr(nullptr)
		, _file_size(0)
	{
		open(filename);
	}

	void open(const char* filename)
	{
		assert(_file_desc == -1);
		int _file_desc = ::open(filename, O_RDONLY);
		if (_file_desc != -1)
		{
			struct stat sb;

			if (fstat(_file_desc, &sb) != -1)
			{
				_file_size = sb.st_size;
				_file_ptr = (char*)mmap(0, _file_size, PROT_READ, MAP_SHARED, _file_desc, 0);
				_file_end = _file_ptr + _file_size;
				_cur_ptr = _file_end;
			}
		}
	}

	~reverse_file_reader()
	{
		if (_file_ptr)
		{
			close(_file_desc);
			munmap(_file_ptr, _file_size);
		}
	}

protected:
	virtual bool process(view& v)
	{
		if (_cur_ptr != _file_ptr)
		{
			char* lineEnd = _cur_ptr[-1] == '\n' ? _cur_ptr - 1 : _cur_ptr;
			char* thisLine = (char*)memrchr(_file_ptr, '\n', lineEnd - _file_ptr);
			if (thisLine)
			{
				thisLine += 1;
			}
			else
			{
				thisLine = _file_ptr;
			}
			
			v.assign(thisLine, _cur_ptr - thisLine);
			_cur_ptr = thisLine;
			
			return true;
		}
		return false;
	}
private:
	int _file_desc;
	char* _file_ptr;
	char* _file_end;
	char* _cur_ptr;
	size_t _file_size;

};

} //namespace perg


