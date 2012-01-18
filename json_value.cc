
#include "json_value.h"
#include "json.h"

json::json_value json::json_value::operator[](const std::string &key) const {
	if(!is_object(*this)) {
		throw invalid_type_cast();
	}

	return (*object_)[key];
}

json::json_value json::json_value::operator[](std::size_t n) const {
	if(!is_array(*this)) {
		throw invalid_type_cast();
	}

	return (*array_)[n];
}
