
#include "json_value.h"
#include "json.h"
#include <boost/make_shared.hpp>

json::null_t json::null;

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

json::json_value::json_value(const json::array &a) {
	type_   = array;
	array_  = boost::make_shared<json::array>(a);
}

json::json_value::json_value(const json::object &a) {
	type_    = object;
	object_  = boost::make_shared<json::object>(a);
}
