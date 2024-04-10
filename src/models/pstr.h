#pragma once


#include <cstring>
#include <iostream>


/*******************************************************************************
*                                                                              *
*                                Pascal String                                 *
*                                                                              *
*******************************************************************************/
template <class t> struct pstr {
	const std::size_t Length() const;
	const char* Print() const;
	const std::size_t Size() const;

	const char* begin() const;
	const char* end() const;

	pstr& operator=(const pstr<t> &p);
	friend std::ostream& operator<<(std::ostream &out, const pstr<t> &p) {
		if (&out == &std::cout)
			out.write(p.data_ + p.prefix_size_, p.prefix_);
		else
			out.write(p.data_, p.prefix_ + p.prefix_size_);

		return out;
	}
	friend std::istream& operator>>(std::istream &in, pstr<t> &p) {
		if (p.data_ != NULL)
			delete[] p.data_;

		in.read(reinterpret_cast<char *>(&p.prefix_), p.prefix_size_);
		in.seekg(in.tellg() - p.prefix_size_);
		p.data_ = new char[p.prefix_size_ + static_cast<size_t>(p.prefix_)];
		in.read(p.data_, p.prefix_size_ + static_cast<size_t>(p.prefix_));

		return in;
	}

	pstr()
		: prefix_(0)
		, prefix_size_(sizeof(t))
		, data_(NULL) { }
	pstr(const char *s)
		: prefix_(std::strlen(s))
		, prefix_size_(sizeof(t)) {
			data_ = new char[prefix_size_ + static_cast<size_t>(prefix_)];
			memcpy(data_, &prefix_, prefix_size_);
			memcpy(data_ + prefix_size_, s, prefix_);
	}
	pstr(const char *s, const std::size_t &p)
		: prefix_(static_cast<t>(p))
		, prefix_size_(sizeof(t)) {
			data_ = new char[prefix_size_ + static_cast<size_t>(prefix_)];
			memcpy(data_, &prefix_, prefix_size_);
			memcpy(data_ + prefix_size_, s, prefix_);
	}
	~pstr() {
	//	if (data_ != NULL)
	//		delete[] data_;
	}

	private:
		t prefix_;
		const std::size_t prefix_size_;
		char *data_;
};

template <class t> const std::size_t pstr<t>::Length() const {
	return prefix_;
}
template <class t> const char* pstr<t>::Print() const {
	return data_ + prefix_size_;
}
template <class t> const std::size_t pstr<t>::Size() const {
	return prefix_size_ + static_cast<size_t>(prefix_);
}
template <class t> const char* pstr<t>::begin() const {
	return data_ + prefix_size_;
}
template <class t> const char* pstr<t>::end() const {
	return data_ + prefix_size_ + prefix_;
}

template<class t> pstr<t>& pstr<t>::operator=(const pstr<t> &p) {
	if (data_ != NULL)
		delete[] data_;

	if ( this != &p ) {
		prefix_ = p.prefix_;
		data_ = new char[prefix_size_ + static_cast<size_t>(prefix_)];
		memcpy(data_, p.data_, prefix_size_ + static_cast<size_t>(prefix_));
	}

	return *this;
}