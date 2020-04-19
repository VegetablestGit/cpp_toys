#ifndef _CSTRING_H_
#define _CSTRING_H_

#include<cstddef>
#include<algorithm>
#include<stdexcept>
#include<cstring>
#include<vector>
#include<iostream>


class CString {

private:
	char* str;
	size_t mysize;
	size_t cap;

	static constexpr size_t init_cap = 8;

	static const char* empty_str() {
		static const char str = 0;
		return &str;
	}

	static size_t str_size(const char* p) {
		size_t s = 0;
		while (*p++) s++;
		return s;
	}

	static void copy_str(char* dest, const char* source) {
		while (*source) *dest++ = *source++;
		*dest = 0;
	}

	static void copy_str(char* dest, const char* source, size_t n) {
		if (dest <= source) {
			while (n--) *dest++ = *source++;
			*dest = 0;
		}
		else {
			dest += n - 1;
			source += n - 1;
			*(dest + 1) = 0;
			while (n--)*dest-- = *source--;
		}
	}

	static char* make_str(const char* source, size_t cap) {
		if (cap == 0) return 0;
		char* str = new char[cap];
		copy_str(str, source);
		return str;
	}

	static const std::vector<int>& get_nextpos(const char* s, size_t n) {
		static std::vector<int> nextpos;
		nextpos.resize(n + 1);
		nextpos[0] = -1;
		int i = 0, j = -1;
		while (i != n) {
			if (j == -1 || s[i] == s[j]) {
				i++, j++;
				if (s[i] == s[j]) nextpos[i] = nextpos[j];
				else nextpos[i] = j;
			}
			else j = nextpos[j];
		}
		return nextpos;
	}

	CString(const char* s,size_t n,size_t cap):CString() {
		str = make_str(s, cap);
		mysize = n;
		this->cap = cap;
	}

	void recap(size_t newcap) { if (newcap != cap) swap(CString(c_str(), size(), newcap)); }

public:
	void swap(CString& s) noexcept {
		std::swap(str, s.str);
		std::swap(mysize, s.mysize);
		std::swap(cap, s.cap);
	}
	void swap(CString&& s) noexcept {
		std::swap(str, s.str);
		std::swap(mysize, s.mysize);
		std::swap(cap, s.cap);
	}

	CString() noexcept :str(0), mysize(0), cap(0) {}
	CString(const char* s, size_t n) :CString() {
		if (n == 0) return;
		str = make_str(s, 2 * n);
		cap = 2 * n;
		mysize = n;
	}
	template<typename it_t>
	CString(it_t first, it_t last) : CString() {
		size_t temp_size = std::distance(first, last);
		if (temp_size == 0) return;
		str = new char[2 * temp_size];
		mysize = temp_size;
		cap = 2 * temp_size;
		std::copy(first, last, str);
		str[mysize] = 0;
	}
	CString(const CString& s) :CString(s.c_str(), s.size()) {}
	CString(CString&& s) noexcept :CString() { swap(s); }
	CString(const char* s) :CString(s, str_size(s)) {}
	CString(size_t n, char c):CString() {
		if (n == 0) return;
		recap(2 * n);
		for (size_t i = 0; i < n; i++) str[i] = c;
		str[n] = 0;
		mysize = n;
	}

	~CString() { if (str) delete[] str; }

	size_t size() const { return str ? mysize : 0; }
	bool empty() const { return size() == 0; }
	void clear() { swap(CString()); }

	char* begin() { if (str) return str; swap(CString(empty_str(), 0, init_cap)); return begin(); }
	const char* begin()const { return str ? str : empty_str(); }
	char* end() { return str ? (str + mysize) : begin(); }
	const char* end() const { return str ? (str + mysize) : empty_str(); }

	CString& operator=(const CString& s) {
		if (this == &s) return *this;
		CString temp(s);
		swap(temp);
		return *this;
	}
	CString& operator=(CString&& s) noexcept {
		if (this == &s) return *this;
		CString temp(std::move(s));
		swap(temp);
		return *this;
	}
	CString& operator=(const char* s) {
		if (s == str) return *this;
		CString temp(s);
		swap(temp);
		return *this;
	}

	const char* c_str()const {
		if (str) return str;
		return empty_str();
	}

	char& operator[](size_t index) {
		return str[index];
	}
	char operator[](size_t index) const {
		return str[index];
	}

	bool operator==(const char* s)const { return strcmp(c_str(), s) == 0; }
	bool operator<(const char* s)const { return strcmp(c_str(), s) < 0; }
	bool operator>(const char* s)const { return strcmp(c_str(), s) > 0; }

	CString substr(size_t pos, size_t n)const {
		if (n + pos > size()) throw std::out_of_range("out of range");
		return CString(begin() + pos, begin() + pos + n);
	}

	CString& insert(size_t pos, const char* s, size_t n) {
		if (pos > size()) throw std::out_of_range("out of range");
		if (n == 0) return *this;
		if (s >= begin() && s < end()) insert(pos, CString(s, n));
		else {
			if (size() + n + 1 > cap) recap(2 * (size() + n));
			copy_str(begin() + pos + n, begin() + pos, size() - pos);
			std::copy(s, s + n, begin() + pos);
		}
		mysize += n;
		return *this;
	}
	CString& insert(size_t pos, const CString& s) { return insert(pos, s.c_str(), s.size()); }
	CString& insert(size_t pos, const char* s) { return insert(pos, s, str_size(s)); }
	CString& insert(size_t pos, char c) { return insert(pos, &c, 1); }

	CString& erase(size_t pos, size_t n) {
		if(pos+n>size()) throw std::out_of_range("out of range");
		mysize -= n;
		copy_str(begin() + pos + n, begin() + pos, n);
		if (size() < cap / 4) recap(cap / 2);
		return *this;
	}

	CString& operator+=(const CString& s) {	return insert(size(), s); }
	CString& operator+=(const char* s) { return insert(size(), s); }
	CString& operator+=(char c) { return insert(size(), c); }
	CString operator+(const CString& s)const { return CString(*this) += s; }
	CString operator+(const char* s)const { return CString(*this) += s; }
	CString operator+(char c)const { return CString(*this) += c; }

	CString& operator*=(size_t n) {
		if (size() == 0) return *this;
		switch (n) {
		case 0: clear();
		case 1: return *this;
		default:
			if (cap < n * size() + 1) recap(2 * n * size());
			for (size_t i = 1; i != n; i++) 
				std::copy(str, str + mysize, str + i * mysize);
			mysize *= n;
			str[mysize] = 0;
			return *this;
		}
	}
	CString operator*(size_t n)const { return CString(*this) *= n; }

	size_t find(size_t pos, const char* pattern,size_t n) const{
		if (n == 0 || pos >= size()) return size();
		const std::vector<int>& nextpos = get_nextpos(pattern, n);
		for (int i = pos, j = 0; i != size();) {
			if (j == -1 || str[i] == pattern[j]) {
				j++, i++;
				if (j == n) return i - n;
			}
			else j = nextpos[j];
		}
		return size();
	}
	size_t find(size_t pos, const char* pattern)const { return find(pos, pattern, str_size(pattern)); }
	size_t find(size_t pos, const CString& pattern)const { return find(pos, pattern.c_str(), pattern.size()); }
};

inline std::ostream& operator<<(std::ostream& os, const CString& s) { return os << s.c_str(); }
inline std::istream& operator>>(std::istream& is, CString& s) {
	s.clear();
	while (isspace(is.peek())) is.get();
	while (!isspace(is.peek())) 
		s += is.get();
	return is;
}

inline bool operator<(const char* l, const CString& r) { return r > l; }
inline bool operator>(const char* l, const CString& r) { return r < l; }
inline bool operator==(const char* l, const CString& r) { return r == l; }

#endif  // _CSTRING_H_