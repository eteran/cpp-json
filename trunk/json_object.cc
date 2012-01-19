
#include "json_object.h"
#include "json_value.h"

json::json_value json::json_object::operator[](const std::string &key) const {
	map_type::const_iterator it = values_.find(key);
	if(it != values_.end()) {
		return it->second;
	}

	return json_value();
}

json::json_object &json::json_object::append(const std::string &key, const json_value &value) {
	values_.insert(std::make_pair(key, value));
	return *this;
}
