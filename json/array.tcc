
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

#if __cplusplus >= 201103L
//------------------------------------------------------------------------------
// Name: array
//------------------------------------------------------------------------------
template <class... Args>
array::array(Args &&...args) {
	values_.reserve(sizeof...(Args));
	array_init(std::forward<Args>(args)...);
}

template <class T, class... Args>
void array::array_init(const T &value, Args &&...args) {
	values_.push_back(json::value(value));
	array_init(args...);
}

template <class T>
void array::array_init(const T &value) {
	values_.push_back(json::value(value));
}
#endif

//------------------------------------------------------------------------------
// Name: array::operator=
//------------------------------------------------------------------------------
inline array &array::operator=(const array &rhs) {
	array(rhs).swap(*this);
	return *this;
}

//------------------------------------------------------------------------------
// Name: operator[]
//------------------------------------------------------------------------------
inline const value array::operator[](std::size_t n) const {
	if(n < values_.size()) {
		return values_[n];
	}
	
	throw invalid_index();
}

//------------------------------------------------------------------------------
// Name: operator[]
//------------------------------------------------------------------------------
inline value &array::operator[](std::size_t n) {
	if(n < values_.size()) {
		return values_[n];
	}
	
	throw invalid_index();
}

//------------------------------------------------------------------------------
// Name: append
//------------------------------------------------------------------------------
template <class T>
inline array &array::append(const T &v) {
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

}

#endif
