#pragma once 
#include <stdlib.h>

namespace tests
{
class temp_file
{
public:
	temp_file()
	{
		char name[] = "/tmp/perg-utests-XXXXXX";
		_file_desc = mkstemp(name);
		_file_name.assign(name);
	}

	~temp_file()
	{
		close(_file_desc);
	}

	void write_line(const char* str)
	{
		::write(_file_desc, str, strlen(str));
		::write(_file_desc, "\n", 1);
		fsync(_file_desc);
	}

	void write(const char* str)
	{
		::write(_file_desc, str, strlen(str));
		fsync(_file_desc);
	}

	const char* name() const
	{
		return _file_name.c_str();
	}
private:
	int _file_desc;
	std::string _file_name;
};
} //namespace tests
