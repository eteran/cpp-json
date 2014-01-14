
#ifndef ARRAY_20120424_TCC_
#define ARRAY_20120424_TCC_

namespace json {

//------------------------------------------------------------------------------
// Name: array
//------------------------------------------------------------------------------
inline array::array() {
}

//------------------------------------------------------------------------------
// Name: array
//------------------------------------------------------------------------------
inline array::array(const array &other) : values_(other.values_) {
}

//------------------------------------------------------------------------------
// Name: array::operator=
//------------------------------------------------------------------------------
inline array &array::operator=(const array &rhs) {
	array(rhs).swap(*this);
	return *this;
}

#if __cplusplus >= 201103L
//------------------------------------------------------------------------------
// Name: array
//------------------------------------------------------------------------------
inline array::array(array &&other) : values_(std::move(other.values_)) {
}

//------------------------------------------------------------------------------
// Name: operator=
//------------------------------------------------------------------------------
inline array &array::operator=(array &&rhs) {
	if(this != &rhs) {
		values_ = std::move(rhs.values_);
	}
	return *this;
}

//------------------------------------------------------------------------------
// Name: append
//------------------------------------------------------------------------------
template <class T, class... Args>
array &array::append(const T &v, Args &&...args) {
	values_.push_back(value(v));
	return append(args...);
}
#endif

//------------------------------------------------------------------------------
// Name: operator[]
//------------------------------------------------------------------------------
inline const value array::operator[](std::size_t n) const {
	return at(n);
}

//------------------------------------------------------------------------------
// Name: operator[]
//------------------------------------------------------------------------------
inline value &array::operator[](std::size_t n) {
	return at(n);
}

//------------------------------------------------------------------------------
// Name: at
//------------------------------------------------------------------------------
inline const value array::at(std::size_t n) const {
	if(n < values_.size()) {
		return values_[n];
	}
	
	throw invalid_index();
}

//------------------------------------------------------------------------------
// Name: at
//------------------------------------------------------------------------------
inline value &array::at(std::size_t n) {
	if(n < values_.size()) {
		return values_[n];
	}
	
	throw invalid_index();
}

//------------------------------------------------------------------------------
// Name: append
//------------------------------------------------------------------------------
template <class T>
array &array::append(const T &v) {
	values_.push_back(value(v));
	return *this;
}

//------------------------------------------------------------------------------
// Name: swap
//------------------------------------------------------------------------------
inline void array::swap(array &other) {
	using std::swap;
	swap(values_, other.values_);
}

//------------------------------------------------------------------------------
// Name: operator==
//------------------------------------------------------------------------------
inline bool operator==(const array &lhs, const array &rhs) {
	if(lhs.values_.size() == rhs.values_.size()) {
		return lhs.values_ == rhs.values_;
	}
	return false;
}

//------------------------------------------------------------------------------
// Name: operator!=
//------------------------------------------------------------------------------
inline bool operator!=(const array &lhs, const array &rhs) {
	return !(lhs == rhs);
}

}

#endif
