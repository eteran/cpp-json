
#ifndef JSON_OBJECT_20120424_TCC_
#define JSON_OBJECT_20120424_TCC_

#include <algorithm>

namespace json {

inline json_object::json_object() {
}

inline json_object::json_object(const json_object &other) : values_(other.values_) {
}

inline json_object &json_object::operator=(const json_object &rhs) {
	json_object(rhs).swap(*this);
	return *this;
}

inline json_value json_object::operator[](const std::string &key) const {
	map_type::const_iterator it = values_.find(key);
	if(it != values_.end()) {
		return it->second;
	}

	return json_value();
}

inline json_object &json_object::append(const std::string &key, const json_value &value) {
	values_.insert(std::make_pair(key, value));
	return *this;
}

inline void json_object::swap(json_object &other) {
	using std::swap;
	swap(values_, other.values_);
}

}

#endif
