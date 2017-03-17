#pragma once
#include "intrusive.h"

namespace perg
{

class slab
{
public:
	slab() : ptr(nullptr), size(0), off(0) {}
	slab(char* ptr, size_t size): ptr(ptr), size(size), off(0) {} 

	size_t avail() const { return size - off;}
	bool full() const { return size == off; }
	bool avail(size_t bytes) const { return avail() >= bytes; }
	void carve(size_t bytes)
	{
		assert(avail(bytes));
		off += bytes;
	}
	void copy(const char* buf, size_t size)
	{
		assert(avail(size));
		memcpy(ptr + off, buf, size);
		carve(size);
	}

	char* memory() { return ptr + off;}

	void clear() { off = 0;}
	size_t occupied() {return off;}
	char* begin() { return ptr; }

private:
	char* ptr;
	size_t size;
	size_t off;
};

class buffer
{
public:

	buffer()
		: _slab_size(0)
		, _occupied(0)
		, _capacity(0)
	{
	}

	~buffer()
	{
		while (!_slabs.empty())
		{
			slab s = _slabs.pop_front();
			s.clear();
			delete[] s.memory();
		}
	}
	
	buffer(const buffer&) = delete;
	buffer& operator =(const buffer&) = delete;

	void copy(const char* ptr, size_t size)
	{
		size_t sz = size;
		while (sz > 0)
		{
			size_t carved = 0;
			char* buf = nullptr;
			carve_some_space(sz, buf, carved);
			memcpy(buf, ptr, carved);
			ptr += carved;
			sz -= carved;
		}
		_occupied += size;
	}

	void copy(const char* str)
	{
		copy(str, strlen(str));
	}

	size_t size() const
	{
		return _occupied;
	}

	size_t capacity() const
	{
		return _capacity;
	}

	template <typename T>
	T convert()
	{
		T t;
		while (!_slabs.empty())
		{
			slab s = _slabs.pop_front();
			t.append(s.begin(), s.occupied());
			delete[] s.begin();
		}

		return t;
	}

private:
	void carve_some_space(size_t requested, char*& buf, size_t& carved)
	{
		if (_slabs.empty() || _slabs.back().full())
		{
			add_slab(requested);	
		}

		slab& recentSlab = _slabs.back();
		carved = std::min(requested, recentSlab.avail());
		buf = recentSlab.memory();
		recentSlab.carve(carved);
	}

	slab& add_slab(size_t size)
	{
		const size_t slabSize = std::max(_slab_size, size);
		char* ptr = new char[slabSize];

		slab newSlab(ptr, slabSize);
		_slabs.push_back(newSlab);
		grow_slab();
		_capacity += slabSize;
		return _slabs.back();
	}

	void grow_slab()
	{
		static const size_t GROW_RATE = 2;
		static const size_t LIMIT = 1024 * 1024;
		_slab_size = std::min(GROW_RATE * _slab_size, LIMIT);
	}

	list<slab> _slabs;
	size_t _slab_size;
	size_t _occupied;
	size_t _capacity;
};

} // namespace perg
