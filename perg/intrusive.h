#pragma once

namespace perg
{
template <typename T>
class list
{
public:
	list()
		: head(nullptr)
		, tail(nullptr)
	{
	}

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
// list<int> llst;
// list<int>::node n(1);
// list<int>::node n(2);
// lst.push_back(n1); lst.push_back(n2);

};
} // namespace perg
