#include<xstddef>
#include <xutility>
#include <iostream>
using namespace::std;
template <typename T>
class my_forward_list {
	struct node {
		T a;
		node* next;
	};
	struct iterator {
		friend class my_forward_list;
		iterator(node* p,my_forward_list<T> *list_p) :p(p),list_p(list_p) {}
		node* operator++() { return p = p->next; }
		node* operator++(int) { node* temp = p; p = p->next; return p; }
		bool operator==(const iterator& a) const { return this->p == a.p; }
		bool operator!=(const iterator& a) const { return !(*this == a); }
		T& operator*() { return p->a; }
		T* operator->() { return &p->a; }
		iterator& advance(size_t n) { while (n--)++* this; return *this; }
		iterator operator+(int diff) { iterator it = *this; return it.advance(diff); }
	private:
		node* p;
		my_forward_list<T>* list_p;
	};
	struct const_iterator {
		friend class my_forward_list;
		const_iterator(const node* p,const my_forward_list<T>* list_p) :p(p), list_p(list_p) {}
		const node* operator++() { return p = p->next; }
		const node* operator++(int) { const node* temp = p; p = p->next; return p; }
		bool operator==(const_iterator& a) const { return this->p == a.p; }
		bool operator!=(const_iterator& a) const { return !(*this == a); }
		const T& operator*() { return p->a; }
		const T* operator->() { return &p->a; }
		const_iterator& advance(size_t n) { while (n--)++* this; }
		const_iterator operator+(int diff) { const_iterator it = *this; return it.advance(diff); }
	private:
		const node* p;
		const my_forward_list<T>* list_p;
	};
public:
	using size_t = unsigned;
	my_forward_list() :head(new node{T(),nullptr}), back(head) {};
	my_forward_list(const initializer_list<T>& a) :my_forward_list() 
	{ for (const auto& i : a) this->push_back(i); }
	my_forward_list(const my_forward_list<T>& a) :my_forward_list() { *this = a; }
	~my_forward_list() { clear(); delete head; }
	my_forward_list<T>& operator=(const my_forward_list<T>& a) {
		if (&a == this) return *this;
		clear();
		for(const auto &i:a)
			push_back(i);
		return *this;
	}
	my_forward_list<T>& operator=(my_forward_list&& a) {
		if (&a == this) return *this;
		clear();
		combine(a);
		return *this;
	}
	bool empty() const { return back == head; }
	iterator before_begin() { return iterator(head,this); }
	iterator begin() { return iterator(head->next,this); }
	const_iterator begin() const { return const_iterator(head->next, this); }
	const_iterator cbegin() const{ return const_iterator(head->next, this); }
	iterator end() { return iterator(nullptr,this); }
	const_iterator end() const { return const_iterator(nullptr, this); }
	const_iterator cend() const { return const_iterator(nullptr, this); }
	void clear() {
		node* p = head->next, * temp;
		while (p) {
			temp = p->next;
			delete p;
			p = temp;
		}
		head->next = nullptr;
		back = head;
	}
	size_t size(){ 
		size_t sum = 0;
		iterator beg = begin(), ed = end();
		while (beg != ed) sum++, beg++;
		return sum;
	}
	iterator insert_after(iterator it, const T& val) {
		node* p = new node{ val,it.p->next };
		it.p->next = p;
		if (back == it.p) back = p;
		return it;
	}
	my_forward_list<T>& push_front(const T& val) {
		insert_after(before_begin(),val);
		return *this;
	}
	my_forward_list<T>& push_back(const T& val) {
		insert_after(iterator(back,this), val);
		return *this;
	}
	iterator erase_after(iterator it) {
		node* temp = it.p->next->next;
		delete it.p->next;
		if (!temp) back = it.p;
		it.p->next = temp;
		return it;
	}
	my_forward_list<T>& combine(my_forward_list& a) {
		if (!a.empty()) {
			this->back->next = a.head->next;
			this->back = a.back;
			a.head->next = nullptr;
			a.back = a.head;
		}
		return *this;
	}
	template<typename C =less<T>>
	my_forward_list<T> sort(const C &cmp= less<T>() ) {
		static auto e = [cmp](const T & a, const T & b)->bool {return cmp(a, b) == cmp(b, a); };
		if (empty()) return *this;
		my_forward_list<T> l, r, mi;
		const T& mid = *begin();
		while (!empty()) {
			if (!e(*begin(), mid)) {
				if (cmp(*begin(), mid)) l.insert_node_after(l.before_begin(), pop_node_after(before_begin()));
				else r.insert_node_after(r.before_begin(), pop_node_after(before_begin()));
			}
			else mi.insert_node_after(mi.before_begin(), pop_node_after(before_begin()));
		}
		l.sort(cmp);
		r.sort(cmp);
		return *this = std::move(l.combine(mi).combine(r));
	}
private:
	node* head, * back;
	node* pop_node_after(iterator it) {
		if (it.p->next == nullptr) return NULL;
		node * p = it.p->next;
		it.p->next = p->next;
		if (p == it.list_p->back) it.list_p->back = it.p;
		return p;
	}
	void insert_node_after(iterator it, node * n) {
		node* temp = it.p->next;
		it.p->next = n;
		n->next = temp;
		if (it.list_p->back == it.p) it.list_p->back = n;
	}
};
