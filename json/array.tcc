
#ifndef ARRAY_20120424_TCC_
#define ARRAY_20120424_TCC_

namespace json {

//------------------------------------------------------------------------------
// Name: son_array()
//------------------------------------------------------------------------------
inline array::array() {
}

//------------------------------------------------------------------------------
// Name: array(const array &other)
//------------------------------------------------------------------------------
inline array::array(const array &other) : values_(other.values_) {
}

//------------------------------------------------------------------------------
// Name: array::operator=(const array &rhs)
//------------------------------------------------------------------------------
inline array &array::operator=(const array &rhs) {
	array(rhs).swap(*this);
	return *this;
}

//------------------------------------------------------------------------------
// Name: operator[](std::size_t n) const
//------------------------------------------------------------------------------
inline const value array::operator[](std::size_t n) const {
	if(n < values_.size()) {
		return values_[n];
	}
	
	throw invalid_index();
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline value &array::operator[](std::size_t n) {
	if(n < values_.size()) {
		return values_[n];
	}
	
	throw invalid_index();
}

//------------------------------------------------------------------------------
// Name: append(const T &v)
//------------------------------------------------------------------------------
template <class T>
inline array &array::append(const T &v) {
	values_.push_back(value(v));
	return *this;
}

//------------------------------------------------------------------------------
// Name: swap(array &other)
//------------------------------------------------------------------------------
inline void array::swap(array &other) {
	using std::swap;
	swap(values_, other.values_);
}

}

#endif
