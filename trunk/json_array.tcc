
#ifndef JSON_ARRAY_20120424_TCC_
#define JSON_ARRAY_20120424_TCC_

#include <algorithm>

namespace json {

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline json_array::json_array() {
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline json_array::json_array(const json_array &other) : values_(other.values_) {
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline json_array &json_array::operator=(const json_array &rhs) {
	json_array(rhs).swap(*this);
	return *this;
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline json_value json_array::operator[](std::size_t n) const {
	if(n >= values_.size()) {
		return json_value();
	}
	
	return values_[n];
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline json_array &json_array::append(const json_value &value) {
	values_.push_back(value);
	return *this;
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline void json_array::swap(json_array &other) {
	using std::swap;
	swap(values_, other.values_);
}

}

#endif
