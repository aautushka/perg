#pragma once
#include "cyclic_list.h"
#include "intrusive.h"

namespace perg
{

// it's basically a list of vectors akin to std::deque
// created for better performance
// we're saving on memory allocations and memory usage
template <typename T, int N = 1024>
class block_list
{
public:
	using self_type = block_list<T, N>;
	block_list()
	{
	}

	block_list(std::initializer_list<T> ll)
	{
		for (auto l : ll) push_back(l);
	}

	~block_list()
	{
	}

	block_list(const self_type&) = delete;
	self_type& operator =(const self_type&) = delete;

	block_list(self_type&& other)
	{
		*this = std::move(other);
	}

	self_type& operator =(self_type&& other)
	{
		_list = std::move(other._list);
		return *this;
	}

	bool empty() const
	{
		return _list.empty();
	}

	size_t size() const
	{
		size_t res = 0;
		for (auto i = _list.begin(); i != _list.end(); ++i)
		//for (const block_t& b : _list)
		{
			res += i->size();
		}

		return res;
	}

	void clear()
	{
		_list.clear();
	}

	void push_back(T t)
	{
		ensure_has_back_space();
		_list.back().push_back(t);
	}

	void push_back(self_type&& tt)
	{
		_list.push_back(std::move(tt._list));
	}

	void push_front(T t)
	{
		ensure_has_front_space();
		_list.front().push_front(t);
	}

	T& emplace_back()
	{
		ensure_has_back_space();
		return _list.back().emplace_back();
	}

	T& emplace_front()
	{
		ensure_has_front_space();
		return _list.front().emplace_front();
	}

	T pop_back()
	{
		assert(!empty());
		block_t& block = _list.back();
		T res = std::move(block.pop_back());
		if (block.empty())
		{
			_list.erase_back();
		}
		return std::move(res);
	}

	T pop_front()
	{
		assert(!empty());
		block_t& block = _list.front();
		T res = std::move(block.pop_front());
		if (block.empty())
		{
			_list.erase_front();
		}
		return std::move(res);
	}

	T& back()
	{
		assert(!empty());
		return _list.back().back(); 
	}

	T& front()
	{
		assert(!empty());
		return _list.front().front();
	}
	
	const T& back() const
	{
		assert(!empty());
		return _list.back().back();
	}

	const T& front() const
	{
		assert(!empty());
		return _list.front().front();
	}


	class iterator
	{
	public:
		explicit iterator(self_type& parent)
			: _iter_list(parent._list.begin())
		{
			if (_iter_list != parent._list.end())
			{
				_iter_block = _iter_list->begin();
			}
		}

		iterator()
		{
		}

		bool operator ==(const iterator& other) const 
		{
			return _iter_list == other._iter_list && _iter_block == _iter_block; 
		}
		bool operator !=(const iterator& other) const 
		{
			return !(*this == other); 
		}
		T& operator *() {return *_iter_block;}
		const T& operator *() const {return *_iter_block;}
		iterator& operator ++() 
		{ 
			static const list_iterator end_list;
			static const block_iterator end_block;
			bool newList = false;
			while (_iter_list != end_list)
			{
				if (newList)
				{
					_iter_block = _iter_list->begin();	
				}
				else
				{
					++_iter_block;
				}

				if (_iter_block != end_block)
				{
					break;	
				}
				else
				{
					++_iter_list;
					newList = true;
				}
			}
			return *this;
		}
		iterator operator ++(int) 
		{ 
			iterator out(*this); ++(*this); return out;
		}
		const T* operator ->() const {return &**this;}
		T* operator ->() {return &**this;}

	private:
		using block_t = cyclic_list<T, N>;
		using list_t = list<block_t>;
		using list_iterator = typename list_t::iterator;
		using block_iterator = typename block_t::iterator;

		list_iterator _iter_list;
		block_iterator _iter_block; 
	};

	iterator begin()
	{
		return iterator(*this);
	}

	iterator end()
	{
		return iterator();
	}
private:
	void ensure_has_back_space()
	{
		if (empty() || _list.back().full())
		{
			_list.emplace_back();
		}
	}

	void ensure_has_front_space()
	{
		if (empty() || _list.front().full())
		{
			_list.emplace_front();
		}
	}

	using block_t = cyclic_list<T, N>;
	using list_t = list<block_t>;

	list_t _list;
};

} // namespace perg

