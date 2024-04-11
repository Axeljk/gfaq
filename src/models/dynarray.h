#pragma once


#include <algorithm>					// std::copy.
#include <initializer_list>				// { } constructor.
#include <iostream>						// iostream overloads.


/*******************************************************************************
*                                                                              *
*                                Dynamic Array                                 *
*                                                                              *
*******************************************************************************/
template <class t> struct dynarray {
	const std::size_t Length() const;
	const std::size_t Size() const;

	t* begin() const;
	t* end() const;

	dynarray& operator=(const dynarray<t> &d);
	t operator[](const int &i) const {
		return this->data_[i];
	}
	t & operator[](const int &i) {
		return this->data_[i];
	}
	friend std::ostream& operator<<(std::ostream &out, const dynarray<t> &d) {
		out.write(reinterpret_cast<const char *>(&d.size_), sizeof(size_));
		for (t i : d)
			out << i;

		return out;
	}
	friend std::istream& operator>>(std::istream &in, dynarray<t> &d) {
		if (d.data_ != NULL)
			delete[] d.data_;

		in.read(reinterpret_cast<char *>(&d.size_), sizeof(size_));
		d.data_ = new t[d.size_];
		for (t &i : d)
			in >> i;

		return in;
	}

	dynarray()
		: size_(0)
		, data_(NULL) { }
	dynarray(std::initializer_list<t> const &d)
		: size_(d.size()) {
			data_ = new t[size_];
			std::copy(d.begin(), d.end(), data_);
			//for (t &i : data_)
			//	data_[i] = d[i];
	}
	~dynarray() {
//		if (data_ != NULL)
//			delete[] data_;
//		size_ = 0;
	}

	private:
		std::size_t size_;
		t *data_;
};

template <class t> const std::size_t dynarray<t>::Length() const {
	return size_;
}
template <class t> const std::size_t dynarray<t>::Size() const {
	return sizeof(size_) + (sizeof(t) * size_);
}
template <class t> t* dynarray<t>::begin() const {
	return data_;
}
template <class t> t* dynarray<t>::end() const {
	return data_ + (size_ > 0 ? size_ : 0);
}

template<class t> dynarray<t>& dynarray<t>::operator=(const dynarray<t> &d) {
	if (data_ != NULL)
		delete[] data_;

	if ( this != &d ) {
		size_ = d.size_;
		data_ = new t[size_];
		std::copy(d.begin(), d.end(), data_);
	}

	return *this;
}