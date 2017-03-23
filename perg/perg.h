#pragma once
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>

#include "pipe.h"
#include "buffer.h"
#include "mask_filter.h"


namespace perg
{

// stupid RAII perf measurement timer
class metric
{
public:
	metric()
	{
		started = std::chrono::steady_clock::now();
	}
	~metric()
	{
		auto stopped = std::chrono::steady_clock::now();
		std::cout << "elapsed " << (stopped - started).count() << std::endl;
	}
private:
	using time_t = std::chrono::time_point<std::chrono::steady_clock>;
	time_t started;
};

// writes all incoming messages into a binary buffer
class search_result : public sink<view>
{
public:
	search_result()
		: _limit(std::numeric_limits<int>::max())
		, _count(0)
		, _separator(0)
	{
	}

	void dump(stream& ss)
	{
		_buffer.dump(ss);
	}

	void limit(int num)
	{
		_limit = num;
	}
	
	void separate_by(char separator)
	{
		_separator = separator; 
	}

protected:
	virtual action process(view& v)
	{
		if (_count < _limit)
		{
			++_count;
			_buffer.copy(v.data(), v.size());

			if (_separator)
			{
				_buffer.copy(&_separator, 1);
			}

			return UNDECIDED;
		}

		return TERMINATE;
	}

private:
	buffer _buffer;
	int _limit;
	int _count;
	char _separator;
};

class file_mapping
{
public:
	file_mapping()
		: _file_desc(-1)
		, _file_ptr(nullptr)
		, _file_size(0)
	{
	}

	explicit file_mapping(const char* filename)
		: _file_desc(-1)
		, _file_ptr(nullptr)
		, _file_size(0)
	{
		open(filename);
	}
	
	file_mapping(const file_mapping&) = delete;
	file_mapping& operator =(const file_mapping&) = delete;

	bool open(const char* filename)
	{
		assert(_file_desc == -1);
		int _file_desc = ::open(filename, O_RDONLY);
		if (_file_desc != -1)
		{
			struct stat sb;

			if (fstat(_file_desc, &sb) != -1)
			{
				_file_size = sb.st_size;
				_file_ptr = (char*)mmap(0, _file_size, PROT_READ, MAP_PRIVATE, _file_desc, 0);
				return true;
			}
		}
		return false;
	}

	~file_mapping()
	{
		if (_file_ptr)
		{
			close(_file_desc);
			munmap(_file_ptr, _file_size);
		}
	}

	size_t size() const { return _file_size; }
	char* data() const { return _file_ptr; }
private:
	int _file_desc;
	char* _file_ptr;
	size_t _file_size;

};



// reads file line by line, every read line translates to 
// a message passed downstream to a filter
// the file is being read in forward direction
class line_reader : public source<view>
{
public:
	line_reader()
		: _file_ptr(nullptr)
		, _file_end(nullptr)
		, _cur_ptr(nullptr)
	{
	}

	explicit line_reader(const char* filename)
		: _file_ptr(nullptr)
		, _file_end(nullptr)
		, _cur_ptr(nullptr)
	{
		open(filename);
	}

	void open(const char* filename)
	{
		if (_mapping.open(filename))
		{
			_cur_ptr = _file_ptr = _mapping.data();
			_file_end = _cur_ptr + _mapping.size();
		}
	}

protected:
	virtual action process(view& v)
	{
		if (_cur_ptr != _file_end)
		{
			char* nextLine = (char*)memchr(_cur_ptr, '\n', _file_end - _cur_ptr);
			size_t size = 0;
			if (nextLine)
			{
				// we do not count the newline char
				size = nextLine++ - _cur_ptr;
			}
			else
			{
				nextLine = _file_end;
				size = nextLine - _cur_ptr;
			}
			
			v.assign(_cur_ptr, size);
			_cur_ptr = nextLine;
			

			return PASS_DOWNSTREAM;
		}
		return TERMINATE;
	}

private:
	file_mapping _mapping;
	char* _file_ptr;
	char* _file_end;
	char* _cur_ptr;

};


// reads file bacwards, the last line goes first
class reverse_line_reader : public source<view>
{
public:
	reverse_line_reader()
		: _file_ptr(nullptr)
		, _file_end(nullptr)
		, _cur_ptr(nullptr)
	{
	}

	explicit reverse_line_reader(const char* filename)
		: _file_ptr(nullptr)
		, _file_end(nullptr)
		, _cur_ptr(nullptr)
	{
		open(filename);
	}

	void open(const char* filename)
	{
		if (_mapping.open(filename))
		{
			_file_ptr = _mapping.data();
			_file_end = _file_ptr + _mapping.size();
			_cur_ptr = _file_end;
		}
	}

protected:
	virtual action process(view& v)
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
			const size_t size = lineEnd - thisLine;
			
			v.assign(thisLine, size);
			_cur_ptr = thisLine;
			
			return PASS_DOWNSTREAM;
		}
		return TERMINATE;
	}
private:
	file_mapping _mapping;
	char* _file_ptr;
	char* _file_end;
	char* _cur_ptr;

};

// reads stdin, and any other FILE* for that matter
class stdin_reader : public source<view>
{
public:
	explicit stdin_reader(FILE* file = stdin)
		: _file(file)
	{
	}

	~stdin_reader()
	{
		while (!_lines.empty())
		{
			char* line = _lines.pop_back();
			free(line);
		}
	}
	
protected:
	virtual action process(view& v)
	{
		size_t size = 0;
		char* ptr = nullptr;
		if (-1 != getline(&ptr, &size, _file))
		{
			size = strlen(ptr) - 1;
			v.assign(ptr, size);
			
			_lines.push_back(ptr);
			return PASS_DOWNSTREAM;
		}
		else
		{
			// man says to free the buffer even if getline fails
			free(ptr);
		}

		return TERMINATE;
	}

private:
	list<char*> _lines;
	FILE* _file;
};

// reads stdin backwards
class reverse_stdin_reader : public source<view>
{
public:
	reverse_stdin_reader(FILE* file = stdin)
		: _done_reading_input(false) 
		, _file(file)
	{
	}

	~reverse_stdin_reader()
	{
		while (!_lines.empty())
		{
			char* line = _lines.pop_back();
			free(line);
		}
	}
	
protected:
	virtual action process(view& v)
	{
		if (!_done_reading_input)
		{
			accumulate_input();
			_done_reading_input = true;
		}
		if (!_lines.empty())
		{
			char* ptr = _lines.pop_front();

			// remove the trailing newline
			size_t len = strlen(ptr) - 1;
			v.assign(ptr, len);
			return PASS_DOWNSTREAM;
		}

		return TERMINATE;
	}

private:
	void accumulate_input()
	{
		size_t size = 0;
		char* ptr = nullptr;
		while ((ptr = nullptr, size = 0) || -1 != getline(&ptr, &size, _file))
		{
			_lines.push_front(ptr);
		}
		// man says to free the buffer even when the call fails 
		free(ptr);
	}
	
	list<char*> _lines;
	bool _done_reading_input;
	FILE* _file;
};

// maps the file add passes the pointer downstream,
// so the downstream filter takes care of it parsing it processing it
class raw_file_reader : public source<view>
{
public:
	raw_file_reader()
		: _cur_ptr(nullptr)
		, _size(0)
	{
	}

	explicit raw_file_reader(const char* filename)
		: _cur_ptr(nullptr)
		, _size(0)
	{
		open(filename);
	}

	void open(const char* filename)
	{
		if (_mapping.open(filename))
		{
			_cur_ptr = _mapping.data();
			_size = _mapping.size();
		}
	}

protected:
	virtual action process(view& v)
	{
		if (_cur_ptr)
		{
			v.assign(_cur_ptr, _size);
			_cur_ptr = nullptr;
			_size = 0;

			return PASS_DOWNSTREAM;
		}
		return TERMINATE;
	}

private:
	file_mapping _mapping;
	char* _cur_ptr;
	size_t _size;
};
} //namespace perg


