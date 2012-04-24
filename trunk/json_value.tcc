
#ifndef JSON_VALUE_20120424_TCC_
#define JSON_VALUE_20120424_TCC_

#include <boost/make_shared.hpp>

namespace json {

inline json_value json_value::operator[](const std::string &key) const {
	if(!is_object(*this)) {
		throw invalid_type_cast();
	}
	
	boost::shared_ptr<json_object> p = boost::get<boost::shared_ptr<json_object> >(value_);
	return (*p)[key];
}

inline json_value json_value::operator[](std::size_t n) const {
	if(!is_array(*this)) {
		throw invalid_type_cast();
	}

	boost::shared_ptr<json_array> p = boost::get<boost::shared_ptr<json_array> >(value_);
	return (*p)[n];
}

inline json_value::json_value(const json_array &a) {
	type_  = array;
	value_ = boost::make_shared<json_array>(a);
}

inline json_value::json_value(const json_object &a) {
	type_  = object;
	value_ = boost::make_shared<json_object>(a);
}

}

#endif
