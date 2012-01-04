
#include "json_object.h"
#include "json_value.h"

json::json_value json::json_object::operator[](const std::string &key) const {
	map_type::const_iterator it = values_.find(key);
	if(it != values_.end()) {
		return it->second;
	}

	return json_value();
}
