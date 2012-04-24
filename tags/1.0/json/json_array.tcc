
#ifndef JSON_ARRAY_20120424_TCC_
#define JSON_ARRAY_20120424_TCC_

namespace json {

//------------------------------------------------------------------------------
// Name: son_array()
//------------------------------------------------------------------------------
inline json_array::json_array() {
}

//------------------------------------------------------------------------------
// Name: json_array(const json_array &other)
//------------------------------------------------------------------------------
inline json_array::json_array(const json_array &other) : values_(other.values_) {
}

//------------------------------------------------------------------------------
// Name: json_array::operator=(const json_array &rhs)
//------------------------------------------------------------------------------
inline json_array &json_array::operator=(const json_array &rhs) {
	json_array(rhs).swap(*this);
	return *this;
}

//------------------------------------------------------------------------------
// Name: operator[](std::size_t n) const
//------------------------------------------------------------------------------
inline const json_value json_array::operator[](std::size_t n) const {
	if(n >= values_.size()) {
		return json_value();
	}
	
	return values_[n];
}

//------------------------------------------------------------------------------
// Name: append(const T &value)
//------------------------------------------------------------------------------
template <class T>
inline json_array &json_array::append(const T &value) {
	values_.push_back(json_value(value));
	return *this;
}

//------------------------------------------------------------------------------
// Name: swap(json_array &other)
//------------------------------------------------------------------------------
inline void json_array::swap(json_array &other) {
	using std::swap;
	swap(values_, other.values_);
}

}

#endif
