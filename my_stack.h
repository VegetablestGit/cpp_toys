#pragma once
#include<cstddef>
#include<utility>
template<typename T>
class my_stack {
	struct node {
		T data;
		node* next;
	};
	node* top_p;
	std::size_t my_size;
public:
	my_stack():top(nullptr),my_size(0){}
	~my_stack() { for (int i = 0, end = my_size; i < end; i++) pop(); }
	bool empty() { return my_size == 0; }
	std::size_t size() { return my_size; }
	void push(const T& val) {
		node* p = new node{ val,top_p };
		top_p = p;
		my_size++;
	}
	void push(T&& val) {
		node* p = new node{ val,top_p };
		top_p = p;
		my_size++;
	}
	void pop() {
		if (!empty()) {
			node* buf = top_p;
			top_p = top_p->next;
			delete buf;
			my_size--;
		}
	}
	T& top(){ if(!empty()) return top_p->data; }
	my_stack<T>& swap(my_stack<T>& a) {
		std::swap(this->size, a.size);
		std::swap(this->top_p, a, a.size_p);
	}
};
template<typename T>
inline void swap(my_stack<T>& a, my_stack<T>& b) { a.swap(b); }