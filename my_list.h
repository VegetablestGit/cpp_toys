#ifndef _MY_LIST_H_
#define _MY_LIST_H_
#include<algorithm>
#include<type_traits>

namespace my {

	template<typename T>
	using is_iterator_t = decltype(std::declval<T>().operator*());


	template<typename T>
	class list {
		struct node {
			node* before, * next;
			T value;
		};
		node* first, * last;
		size_t my_size;
	public:
		list() :first(new node{ 0,0,T() }), last(new node{ 0,0,T() }), my_size(0) { first->next = last; last->before = first; }
		template<typename it, class = is_iterator_t<it>> list(it fisrt, it last) : list() { insert(end(), fisrt, last); }
		list(std::initializer_list<T> list) : list(list.begin(), list.end()) {}
		list(size_t n, T val = T()) :list() { while (my_size != n) push_back(val); }
		list(const list<T>& rhs) :list(rhs.begin(), rhs.end()) {}
		list(list<T>&& rhs) :list() { swap(rhs); }

		~list() {
			node* nodep = first->next;
			while (nodep) {
				delete nodep->before;
				nodep = nodep->next;
			}
			delete last;
		}

		void swap(list<T>& rhs) {
			std::swap(first, rhs.first);
			std::swap(last, rhs.last);
			std::swap(my_size, rhs.my_size);
		}

		void swap(list<T>&& rhs) {
			std::swap(first, rhs.first);
			std::swap(last, rhs.last);
			std::swap(my_size, rhs.my_size);
		}

		class const_iterator {
			friend class list;
		protected:
			node* nodep = 0;
			const_iterator(node* nodep) :nodep(nodep) {}
		public:
			const_iterator& operator++() { nodep = nodep->next; return *this; }
			const_iterator& operator++(int) { const_iterator ret = *this; nodep = nodep->next; return ret; }
			const_iterator& operator--() { nodep = nodep->before; return *this; }
			const_iterator& operator--(int) { const_iterator ret = *this; nodep = nodep->before; return ret; }
			bool operator==(const const_iterator& rhs) { return nodep == rhs.nodep; }
			bool operator!=(const const_iterator& rhs) { return nodep != rhs.nodep; }
			const T& operator*() { return nodep->value; }
			const T* operator->() { return &nodep->value; }
		};
		class iterator :public const_iterator {
			friend class list;
			iterator(node* nodep) :const_iterator(nodep) {}
		public:
			iterator& operator++() { this->nodep = this->nodep->next; return *this; }
			iterator& operator++(int) { iterator ret = *this; this->nodep = this->nodep->next; return ret; }
			iterator& operator--() { this->nodep = this->nodep->before; return *this; }
			iterator& operator--(int) { iterator ret = *this; this->nodep = this->nodep->before; return ret; }
			T& operator*() { return this->nodep->value; }
			T* operator->() { return &this->nodep->value; }
		};

		iterator begin() { return first->next; }
		const_iterator begin()const { return first->next; }
		iterator end() { return last; }
		const_iterator end()const { return last; }

		size_t size()const { return my_size; }
		bool empty()const { return my_size == 0; }

		iterator insert(const_iterator pos, const T& val) {
			node* nodep = pos.nodep;
			node* new_nodep = new node{ nodep->before,nodep,val };
			new_nodep->before->next = new_nodep;
			nodep->before = new_nodep;
			my_size++;
			return nodep;
		}
		iterator insert(const_iterator pos, T&& val) {
			node* nodep = pos.nodep;
			node* new_nodep = new node{ nodep->before,nodep,std::move(val) };
			new_nodep->before->next = new_nodep;
			nodep->before = new_nodep;
			my_size++;
			return nodep;
		}
		template<typename it>
		iterator insert(const_iterator pos, it first, it last) {
			node* nodep = pos.nodep->before;
			while (first != last) {
				node* new_nodep = new node{ nodep,0,*first };
				nodep->next = new_nodep;
				nodep = new_nodep;
				my_size++;
				++first;
			}
			nodep->next = pos.nodep;
			pos.nodep->before = nodep;
			return iterator(pos.nodep);
		}

		iterator erase(const_iterator pos) {
			pos.nodep->before->next = pos.nodep->next;
			pos.nodep->next->before = pos.nodep->before;
			node* ret = pos.nodep->next;
			delete pos.nodep;
			my_size--;
			return ret;
		}

		void push_back(const T& val) { insert(end(), val); }
		void push_back(T&& val) { insert(end(), std::move(val)); }
		void push_front(const T& val) { insert(begin(), val); }
		void push_front(T&& val) { insert(begin(), std::move(val)); }
		void pop_back() { erase(iterator(last->before)); }
		void pop_front() { erase(begin()); }

		void clear() { swap(list<T>()); }

		list<T> operator=(const list<T>& rhs) { swap(list<T>(rhs)); }
		list<T> operator=(list<T>&& rhs) { swap(rhs); }

		template<typename COMP = std::less<T>>
		void sort(COMP comp = std::less<T>()) {
			if (my_size == 0) return;
			node** nodep_array = new node * [my_size];
			node** nodepp = nodep_array;
			iterator it = begin();
			for (size_t i = 0; i < my_size; i++, ++it, nodepp++)
				*nodepp = it.nodep;
			std::sort(nodep_array, nodep_array + my_size, [=](node* nodep_l, node* nodep_r) {return comp(nodep_l->value, nodep_r->value); });
			it = first;
			nodepp = nodep_array;
			for (size_t i = 0; i < my_size; i++, ++it, nodepp++) {
				it.nodep->next = *nodepp;
				(*nodepp)->before = it.nodep;
			}
			it.nodep->next = last;
			last->before = it.nodep;
			delete[]nodep_array;
		}
	};

}

#endif //_MY_LIST_H_