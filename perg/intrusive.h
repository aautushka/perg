#pragma once

namespace perg
{
template <typename T>
class intrusive_list
{
public:
	intrusive_list()
		: head(nullptr)
		, tail(nullptr)
	{
	}

	intrusive_list(const intrusive_list<T>&) = delete;
	intrusive_list<T>& operator =(const intrusive_list<T>&) = delete;

	struct node
	{
		node() : prev(nullptr), next(nullptr){}
		node(T t) : data(std::move(t)){}
		node* prev;
		node* next;
		T data;

		operator T() const
		{
			return data;
		}
	};

	bool empty() const
	{
		return head == nullptr;
	}

	size_t size() const
	{
		size_t res = 0;
		node* cur = head;
		while (cur != tail)
		{
			res += 1;
			cur = cur->next;	
		}	
		
		res += (head == nullptr ? 0 : 1);
		return res;
	}
	
	void push_back(node& n)
	{
		if (empty())
		{
			init(n);
		}
		else
		{
			tail->next = &n;
			n.next = nullptr;
			n.prev = tail;
			tail = &n;
		}
	}

	void push_front(node& n)
	{
		if (empty())
		{
			init(n);
		}
		else
		{
			head->prev = &n;
			n.prev = nullptr;
			n.next = head;
			head = &n;
		}
	}	

	node& pop_back()
	{
		assert(tail != nullptr);
		node* res = tail;

		if (tail->prev)
		{
			tail->prev->next = nullptr;
			tail = tail->prev;
		}
		else
		{
			clear();
		}
		res->prev = nullptr;
		return *res;
	}

	node& pop_front()
	{
		assert(head != nullptr);
		node* res = head;
		
		if (head->next)
		{
			head->next->prev = nullptr;
			head = head->next;
		}
		else
		{
			clear();
		}
		res->next = nullptr;
		return *res;
	}

	void clear()
	{
		tail = head = nullptr;
	}

	const node& front() const
	{
		assert(head != nullptr);
		return *head;
	}


	const node& back() const
	{
		assert(tail != nullptr);
		return *tail;	
	}	


	node& front() 
	{
		assert(head != nullptr);
		return *head;
	}

	node& back()
	{
		assert(tail != nullptr);
		return *tail;
	}

	class iterator
	{
	public:
		iterator() : _cur(nullptr) { }
		explicit iterator(intrusive_list<T>& l) : _cur(l.head) { }
		bool operator ==(const iterator& other) const {return _cur == other._cur; }
		bool operator !=(const iterator& other) const { return !(*this == other); }
		T& operator *() {return _cur->data; }
		const T& operator *() const {return _cur->data;}
		iterator& operator ++() { _cur = _cur->next; return *this; }
		iterator operator ++(int) { iterator out(*this); ++(*this); return out;}
		T* operator ->() { return &**this; }
		const T* operator ->() const {return &**this;}

	private:
		typename intrusive_list<T>::node* _cur;
	};

	iterator begin() { return iterator(*this); }
	iterator end() { return iterator(); }

	
private:
	void init(node& n)
	{
		head = &n;
		tail = &n;
		head->next = nullptr;
		head->prev = nullptr;
	}

	node* head;
	node* tail;
};

template <typename T>
class list
{
public:
	list()
	{
	}

	list(const list<T>&) = delete;
	list<T>& operator =(const list<T>&) = delete;

	void push_back(T t)
	{
		node_t* node = new node_t(std::move(t));
		_list.push_back(*node);
	}

	void push_front(T t)
	{
		node_t* node = new node_t(std::move(t));
		_list.push_front(*node);
	}

	bool empty() const
	{
		return _list.empty();
	}

	size_t size() const
	{
		return _list.size();
	}

	void clear(){ _list.clear(); }

	T pop_front()
	{
		node_t& node = _list.pop_front();
		T t = std::move(node.data);
		delete &node;
		return std::move(t);
	}

	T pop_back()
	{
		node_t& node = _list.pop_back();
		T t = std::move(node.data);
		delete &node;
		return std::move(t);
	}

	T& back() { return _list.back().data; }
	T& front() {return _list.front().data; }
	const T& back() const {return _list.back().data; }
	const T& front() const {return _list.front().data; }


	using iterator = typename intrusive_list<T>::iterator;

	iterator begin()
	{
		return _list.begin();
	}

	iterator end()
	{
		return _list.end();
	}

private:
	using list_t = intrusive_list<T>;
	using node_t = typename list_t::node;
	list_t _list;
};

} // namespace perg
