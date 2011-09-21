#include "json.h"


json::value::value() {
}

json::value::value(const boost::shared_ptr<json_value> &v) : value_(v) {
}

json::value json::value::operator[](const std::string &key) const {
	if(!is_object(*this)) {
		throw invalid_type_cast();
	}
	return (*(value_->object_))[key];
}

json::value json::value::operator[](std::size_t n) const {
	if(!is_array(*this)) {
		throw invalid_type_cast();
	}
	return (*value_->array_)[n];
}

bool json::is_string(const value &v) {
	return (v.value_->type_ == json_value::string);
}

bool json::is_number(const value &v) {
	return (v.value_->type_ == json_value::number);
}

bool json::is_object(const value &v) {
	return (v.value_->type_ == json_value::object);
}

bool json::is_array(const value &v) {
	return (v.value_->type_ == json_value::array);
}

bool json::is_null(const value &v) {
	return (v.value_->type_ == json_value::null);
}

bool json::is_bool(const value &v) {
	return (v.value_->type_ == json_value::boolean);
}

std::string json::to_string(const value &v) {
	if(!is_string(v) && !is_bool(v) && !is_number(v)) {
		throw invalid_type_cast();
	}
	return v.value_->token_.string_;
}

bool json::to_bool(const value &v) {
	if(!is_bool(v)) {
		throw invalid_type_cast();
	}
	return v.value_->token_.string_ == "true";
}

double json::to_number(const value &v) {
	if(!is_number(v)) {
		throw invalid_type_cast();
	}
	return strtod(v.value_->token_.string_.c_str(), 0);
}

size_t json::size(const value &v) {
	if(is_array(v)) {
		return v.value_->array_->values_.size();
	}
	
	return 0;
}
