
#ifndef JSON_OBJECT_20120424_TCC_
#define JSON_OBJECT_20120424_TCC_

#include <algorithm>

namespace json {

//------------------------------------------------------------------------------
// Name: json_object()
//------------------------------------------------------------------------------
inline json_object::json_object() {
}

//------------------------------------------------------------------------------
// Name: json_object(const json_object &other)
//------------------------------------------------------------------------------
inline json_object::json_object(const json_object &other) : values_(other.values_) {
}

//------------------------------------------------------------------------------
// Name: operator=(const json_object &rhs)
//------------------------------------------------------------------------------
inline json_object &json_object::operator=(const json_object &rhs) {
	json_object(rhs).swap(*this);
	return *this;
}

//------------------------------------------------------------------------------
// Name: operator[](const std::string &key) const
//------------------------------------------------------------------------------
inline json_value json_object::operator[](const std::string &key) const {
	map_type::const_iterator it = values_.find(key);
	if(it != values_.end()) {
		return it->second;
	}

	return json_value();
}

//------------------------------------------------------------------------------
// Name: append(const std::string &key, const T &value)
//------------------------------------------------------------------------------
template <class T>
inline json_object &json_object::append(const std::string &key, const T &value) {
	values_.insert(std::make_pair(key, json_value(value)));
	return *this;
}

//------------------------------------------------------------------------------
// Name: swap(json_object &other)
//------------------------------------------------------------------------------
inline void json_object::swap(json_object &other) {
	using std::swap;
	swap(values_, other.values_);
}

}

#endif
